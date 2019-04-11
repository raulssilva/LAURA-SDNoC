#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <math.h>

// Network's dimensions
#define N 2 // Widith
#define M 2 // Height

#define CHANNEL_WIDITH 32
#define ROUTERS_SWITCHERS 10
#define ROUTERS_ENABLES 5
/*#define CORE_ID (1 + 1) /*
						 * Seguir a quantidade de roteadores por linhas e colunas (l + c)
						 * (int)log2(N) < log2(N) ? (int)log2(N) + 1 : (int)log2(N)
						 * (int)log2(M) < log2(M) ? (int)log2(M) + 1 : (int)log2(M)
						 */
// #define CORES_REQUESTS CORE_ID + 1 // The less significant bit defines if there is a request

#endif