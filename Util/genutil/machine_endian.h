#ifndef __M_ENDIAN_H
#define __M_ENDIAN_H

/* the main byte-swapping function */
#ifdef __cplusplus
extern "C" {
#endif

void changeEndian (void *p, int size);
void array_changeEndian (int count, void *p, int element_size);

#ifdef __cplusplus
}
#endif

/* is it Big or Little endian?? */
#ifdef __alpha
#ifndef MACHINE_LITTLE_ENDIAN
#define MACHINE_LITTLE_ENDIAN
#endif
#endif

#ifdef sgi
#undef MACHINE_LITTLE_ENDIAN
#endif

#ifdef sun
#define MACHINE_BIG_ENDIAN
#endif

#ifdef __linux
#define MACHINE_LITTLE_ENDIAN
#endif

/* endian-ness macros */
#ifdef MACHINE_LITTLE_ENDIAN

#define toBigEndian(p,s) 	changeEndian(p,s)
#define toLittleEndian(p,s)
#define fromBigEndian(p,s)	changeEndian(p,s)
#define fromLittleEndian(p,s)

/* short forms that save typing */
#define ToBigEndian(p)          changeEndian (&(p), sizeof(p))
#define ToLittleEndian(p)
#define FromBigEndian(p)        changeEndian (&(p), sizeof(p))
#define FromLittleEndian(p)   

/* short forms that work on arrays */
#define N_ToBigEndian(N,p)      array_changeEndian (N, p, sizeof(*(p)))
#define N_ToLittleEndian(N,p)
#define N_FromBigEndian(N,p)    array_changeEndian (N, p, sizeof(*(p)))
#define N_FromLittleEndian(N,p)

#else

#define toBigEndian(p,s)
#define toLittleEndian(p,s)	changeEndian(p,s)
#define fromBigEndian(p,s)		
#define fromLittleEndian(p,s)	changeEndian(p,s)

/* short forms that save typing */
#define ToBigEndian(p)
#define ToLittleEndian(p)       changeEndian (&(p), sizeof(p))
#define FromBigEndian(p)
#define FromLittleEndian(p)     changeEndian (&(p), sizeof(p))

/* short forms that work on arrays */
#define N_ToBigEndian(N,p) 
#define N_ToLittleEndian(N,p)   array_changeEndian (N, p, sizeof(*p))
#define N_FromBigEndian(N,p) 
#define N_FromLittleEndian(N,p) array_changeEndian (N, p, sizeof(*p))

#endif

#endif
