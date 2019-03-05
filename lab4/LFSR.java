// Oct 13, 2016.
// @maxkrivich
// number theory and applied cryptography
// Berlekamp–Massey algorithm attack on the LFSR


import java.util.*;

public class LFSR {

    private final int[] register;
    private final int[] polinomia;
    private final int lengthOfReg;

    public LFSR(int[] polinomia, int length) {
        this.lengthOfReg = length;
        System.arraycopy(polinomia, 0, this.polinomia, 0, polinomia.length);
        this.register = new int[this.lengthOfReg];
        Arrays.fill(this.register, 1);
    }

    public int[] generate(int steps) {
        int[] res = new int[steps];
        for (int t = 0; t < steps; t++)
            res[t] = generateNext();
        return res;
    }

    public int generateNext() {
        int next = register[lengthOfReg - 1];
        for (int i = 1; i < polinomia.length; i++)
            next ^= register[polinomia[i] - 1];
        for (int i = lengthOfReg - 1; i > 0; i--)
            register[i] = register[i - 1];
        register[0] = next;
        return next;
    }

    private String regStat() {
        return Arrays.toString(register).substring(1).replaceFirst("]", "").replace(", ", "");
    }

    public void printReg() {
        System.out.println(regStat());
    }

    public long getPeriod() {
        return (1 << lengthOfReg) - 1;
    }

    public int getTwoM() {
        return polinomia[0] * 2;
    }

    @Override
    public String toString() {
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < polinomia.length; i++)
            sb.append(String.format("x^%d+", polinomia[i]));
        sb.append("1");
        return sb.toString();
    }

    public static void main(String[] args) {
        LFSR l = new LFSR(new int[] {13, 4, 3, 1}, 13);
        System.out.printf("PERIOD:\t%d\nPOLINOME:\t%s\n", l.getPeriod(), l);
        LFSRAttack la = new LFSRAttack(l.generate(l.getTwoM()));
        la.attack();
        System.out.printf("ATTACK POLINOME:\t%s\n", la);
        for(int i : l.generate(l.getTwoM()))
            System.out.print(i);
        System.out.println();
    }
}


class LFSRAttack {

    private int[] a;
    private int[] res;

    LFSRAttack() {

    }

    LFSRAttack(int[] a) {
        this.setArray(a);
    }

    public void setArray(int[] a) {
        this.a = new int[a.length];
        System.arraycopy(a, 0, this.a, 0, a.length);
    }

    public int[] attack() { // Berlekamp–Massey algorithm
        final int N = a.length;
        int[] b = new int[N];
        int[] c = new int[N];
        int[] t = new int[N];
        b[0] = 1;
        c[0] = 1;
        int l = 0;
        int m = -1;
        for (int n = 0; n < N; n++) {
            int d = 0;
            for (int i = 0; i <= l; i++)
                d ^= c[i] * a[n - i];
            if (d == 1) {
                System.arraycopy(c, 0, t, 0, N);
                for (int j = 0; j < N - n + m; j++)
                    c[n - m + j] ^= b[j];
                if (l <= n / 2) {
                    l = n + 1 - l;
                    m = n;
                    System.arraycopy(t, 0, b, 0, N);
                }
            }
        }
        res = new int[N];
        System.arraycopy(c, 0, res, 0, N);
        return c;
    }

    @Override
    public String toString() {
        if (res.length == 0)
            return "";
        StringBuffer sb = new StringBuffer();
        for (int i = res.length - 1; i >= 1; i--)
            if (res[i] == 1)
                sb.append(String.format("x^%d+", i));
        sb.append("1");
        return sb.toString();
    }

}