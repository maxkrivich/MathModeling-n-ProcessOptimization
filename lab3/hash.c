#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define BLOCK_SIZE 16  // bits
#define MESSAGE_SIZE 64  // bits
#define HASH_RAND_MIN 0  // minimum value
#define HASH_RAND_MAX 18446744073709551615LLU  // 2^64 -1 maximum value


uint64_t randInt(uint64_t upper, uint64_t lower)
{
    return (rand() % (upper - lower)) + lower;
}

uint64_t* genArray(uint64_t size)
{
    uint64_t* arr = (uint64_t*) malloc(sizeof(uint64_t)*size);

    for(int i = 0; i < size; i++)
        arr[i] = randInt(HASH_RAND_MAX, HASH_RAND_MIN);

    return arr;
}

uint16_t hash(uint64_t message)
{
    // printf("%llu\n", message);
    uint16_t m0 = (message & 0xFFFF),
             m1 = (message >> 16) & 0xFFFF,
             m2 = (message >> 32) & 0xFFFF,
             m3 = (message >> 48) & 0xFFFF;
    // return  (m0 & m1) | (m2 ^ m3); 
    return  (m0 | m1) ^ (m2 & m3);
}

uint32_t hammingDistance(uint16_t n1, uint16_t n2) 
{ 
    uint16_t x = n1 ^ n2; 
    uint32_t setBits = 0; 
  
    while (x > 0) { 
        setBits += x & 1; 
        x >>= 1; 
    } 
    return setBits; 
}


double claclulateAvalancheEffect(uint32_t n)
{
    double cnt = 0;
    for(uint32_t i = 0; i < n; i++)
    {
        uint64_t m = randInt(HASH_RAND_MAX, HASH_RAND_MIN);
        uint16_t h = hash(m), tmp_hash;
        uint64_t step = 1;
        for (uint64_t j = 0; j < MESSAGE_SIZE; j++)
        {
            tmp_hash = hash(m ^ step);
            cnt += hammingDistance(h, tmp_hash);
            step <<= 1;
        }
    }
    return cnt / (n * BLOCK_SIZE * MESSAGE_SIZE);

}

double* collisionBirthdayParadox(uint32_t n, uint32_t kMax)
{
    double* res = (double*) malloc(sizeof(double) * kMax);
    for(uint32_t k = 0; k < kMax; k++)
    {
        double cnt = 0;
        for(uint32_t j = 0; j < n; j++)
        {
            uint16_t* hashes = (uint16_t*) calloc(1 << BLOCK_SIZE, sizeof(uint16_t));
            uint64_t* messages = (uint64_t*) malloc(sizeof(uint64_t)*(k+1));
            for(uint32_t z = 0; z < k; z++)
            {
                messages[z] = randInt(HASH_RAND_MAX, HASH_RAND_MIN);
                uint16_t h = hash(messages[z]);
                // printf("%u %u\n", k, h);
                if(hashes[h] == 0)
                {
                    hashes[h] = z;
                } else if (messages[z] != messages[hashes[h]])
                {
                    cnt++;
                    break;
                } 
            }
            free(hashes);
            free(messages);
        }
        cnt /= n;
        res[k] = cnt;
    }
    return res;
}

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        fprintf(stderr, "Datafile is not specified\n");
        return -1;
    }
    srand(time(NULL));

    // Task 1.2.2
    uint32_t experiments = 1000, maxK = 200;
    fprintf(stdout, "AvalancheEffect(%d) = %g\n", experiments, claclulateAvalancheEffect(experiments));

    // Task 1.2.3
    double* res = collisionBirthdayParadox(experiments, maxK);
    FILE* f = fopen(argv[1], "w");
    if (!f)
    {
        fprintf(stderr, "Could not open file\n"); 
        return -1;
    }
    for(uint32_t i = 0; i < maxK; i++)
        fprintf(f, "%d\t%g\n", i, res[i]);

    free(res);
    fclose(f);
    return 0;
}