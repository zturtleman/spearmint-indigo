/*
===========================================================================
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.

This file is part of Spearmint Source Code.

Spearmint Source Code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Spearmint Source Code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Spearmint Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, Spearmint Source Code is also subject to certain additional terms.
You should have received a copy of these additional terms immediately following
the terms and conditions of the GNU General Public License.  If not, please
request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
//
#ifndef __Q_SHARED_H
#define __Q_SHARED_H

// q_shared.h -- included first by ALL program modules.
// A user mod should never modify this file

#define PRODUCT_NAME				"Spearmint"
#define HOMEPATH_NAME_UNIX			".spearmint"
#define HOMEPATH_NAME_WIN			"Spearmint"
#define HOMEPATH_NAME_MACOSX		HOMEPATH_NAME_WIN
#define GAMENAME_FOR_MASTER			"Spearmint"	// must NOT contain whitespace

#ifndef BASEGAME
	#define BASEGAME				"baseq3"
#endif

#ifndef PRODUCT_VERSION
	#define PRODUCT_VERSION "Alpha"
#endif

// In the future if the client-server protocol is modified, this may allow old and new engines to play together
//#define LEGACY_PROTOCOL

// Heartbeat for dpmaster protocol. You shouldn't change this unless you know what you're doing
#define HEARTBEAT_FOR_MASTER		"DarkPlaces"
#define FLATLINE_FOR_MASTER			HEARTBEAT_FOR_MASTER

// id Software games to not auto download
#define BASEQ3				"baseq3"
#define BASETA				"missionpack"

#define Q3_VERSION PRODUCT_NAME " " PRODUCT_VERSION

#define MAX_TEAMNAME		32
#define MAX_MASTER_SERVERS      5	// number of supported master servers

#define DEMOEXT	"dm_"			// standard demo extension

#ifdef _MSC_VER

#pragma warning(disable : 4018)     // signed/unsigned mismatch
#pragma warning(disable : 4032)
#pragma warning(disable : 4051)
#pragma warning(disable : 4057)		// slightly different base types
#pragma warning(disable : 4100)		// unreferenced formal parameter
#pragma warning(disable : 4115)
#pragma warning(disable : 4125)		// decimal digit terminates octal escape sequence
#pragma warning(disable : 4127)		// conditional expression is constant
#pragma warning(disable : 4136)
#pragma warning(disable : 4152)		// nonstandard extension, function/data pointer conversion in expression
//#pragma warning(disable : 4201)
//#pragma warning(disable : 4214)
#pragma warning(disable : 4244)
#pragma warning(disable : 4142)		// benign redefinition
//#pragma warning(disable : 4305)		// truncation from const double to float
//#pragma warning(disable : 4310)		// cast truncates constant value
//#pragma warning(disable:  4505) 	// unreferenced local function has been removed
#pragma warning(disable : 4514)
#pragma warning(disable : 4702)		// unreachable code
#pragma warning(disable : 4711)		// selected for automatic inline expansion
#pragma warning(disable : 4220)		// varargs matches remaining parameters
//#pragma intrinsic( memset, memcpy )
#endif

//Ignore __attribute__ on non-gcc platforms
#ifndef __GNUC__
#ifndef __attribute__
#define __attribute__(x)
#endif
#endif

#ifdef __GNUC__
#define UNUSED_VAR __attribute__((unused))
#else
#define UNUSED_VAR
#endif

#if (defined _MSC_VER)
#define Q_EXPORT __declspec(dllexport)
#elif (defined __SUNPRO_C)
#define Q_EXPORT __global
#elif ((__GNUC__ >= 3) && (!__EMX__) && (!sun))
#define Q_EXPORT __attribute__((visibility("default")))
#else
#define Q_EXPORT
#endif

/**********************************************************************
  VM Considerations

  The VM can not use the standard system headers because we aren't really
  using the compiler they were meant for.  We use bg_lib.h which contains
  prototypes for the functions we define for our own use in bg_lib.c.

  When writing mods, please add needed headers HERE, do not start including
  stuff like <stdio.h> in the various .c files that make up each of the VMs
  since you will be including system headers files can will have issues.

  Remember, if you use a C library function that is not defined in bg_lib.c,
  you will have to add your own version for support in the VM.

 **********************************************************************/

#ifdef Q3_VM

#include "../game/bg_lib.h"

typedef int intptr_t;

#else

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>

#ifdef _MSC_VER
  #include <io.h>

  typedef __int64 int64_t;
  typedef __int32 int32_t;
  typedef __int16 int16_t;
  typedef __int8 int8_t;
  typedef unsigned __int64 uint64_t;
  typedef unsigned __int32 uint32_t;
  typedef unsigned __int16 uint16_t;
  typedef unsigned __int8 uint8_t;

  // vsnprintf is ISO/IEC 9899:1999
  // abstracting this to make it portable
  int Q_vsnprintf(char *str, size_t size, const char *format, va_list ap);

  #define rint(x) (floor(x)+0.5f)
#else
  #include <stdint.h>

  #define Q_vsnprintf vsnprintf
#endif

#endif


#include "q_platform.h"

//=============================================================

typedef unsigned char 		byte;

typedef enum {qfalse, qtrue}	qboolean;

typedef union {
	float f;
	int i;
	unsigned int ui;
} floatint_t;

typedef int		qhandle_t;
typedef int		sfxHandle_t;
typedef int		fileHandle_t;
typedef int		clipHandle_t;

#define PAD(base, alignment)	(((base)+(alignment)-1) & ~((alignment)-1))
#define PADLEN(base, alignment)	(PAD((base), (alignment)) - (base))

#define PADP(base, alignment)	((void *) PAD((intptr_t) (base), (alignment)))

#ifdef __GNUC__
#define QALIGN(x) __attribute__((aligned(x)))
#else
#define QALIGN(x)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define STRING(s)			#s
// expand constants before stringifying them
#define XSTRING(s)			STRING(s)

#define	MAX_QINT			0x7fffffff
#define	MIN_QINT			(-MAX_QINT-1)

#define ARRAY_LEN(x)			(sizeof(x) / sizeof(*(x)))
#define STRARRAY_LEN(x)			(ARRAY_LEN(x) - 1)

// angle indexes
#define	PITCH				0		// up / down
#define	YAW					1		// left / right
#define	ROLL				2		// fall over

// the game guarantees that no string from the network will ever
// exceed MAX_STRING_CHARS
#define	MAX_STRING_CHARS	1024	// max length of a string passed to Cmd_TokenizeString
#define	MAX_STRING_TOKENS	1024	// max tokens resulting from Cmd_TokenizeString
#define	MAX_TOKEN_CHARS		1024	// max length of an individual token

#define	MAX_INFO_STRING		1024
#define	MAX_INFO_KEY		  1024
#define	MAX_INFO_VALUE		1024

#define	BIG_INFO_STRING		8192  // used for system info key only
#define	BIG_INFO_KEY		  8192
#define	BIG_INFO_VALUE		8192


#define	MAX_QPATH			64		// max length of a quake game pathname
#ifdef PATH_MAX
#define MAX_OSPATH			PATH_MAX
#else
#define	MAX_OSPATH			256		// max length of a filesystem pathname
#endif

#define	MAX_NAME_LENGTH		32		// max length of a client name

#define	MAX_SAY_TEXT	150

// paramters for command buffer stuffing
typedef enum {
	EXEC_NOW,			// don't return until completed, a VM should NEVER use this,
						// because some commands might cause the VM to be unloaded...
	EXEC_INSERT,		// insert at current position, but don't run yet
	EXEC_APPEND			// add to end of the command buffer (normal case)
} cbufExec_t;


//
// these aren't needed by any of the VMs.  put in another header?
//
#define	MAX_MAP_AREA_BYTES		32		// bit vector of area visibility


// print levels from renderer (FIXME: set up for game / cgame?)
typedef enum {
	PRINT_ALL,
	PRINT_DEVELOPER,		// only print when "developer 1"
	PRINT_WARNING,
	PRINT_ERROR
} printParm_t;


#ifdef ERR_FATAL
#undef ERR_FATAL			// this is be defined in malloc.h
#endif

// parameters to the main Error routine
typedef enum {
	ERR_FATAL,					// exit the entire game with a popup window
	ERR_DROP,					// print to console and disconnect from game
	ERR_SERVERDISCONNECT,		// don't kill server
	ERR_DISCONNECT,				// client disconnected from the server
} errorParm_t;


#if !defined(NDEBUG) && !defined(BSPC)
	#define HUNK_DEBUG
#endif

typedef enum {
	h_high,
	h_low,
	h_dontcare
} ha_pref;

#ifdef HUNK_DEBUG
#define Hunk_Alloc( size, preference )				Hunk_AllocDebug(size, preference, #size, __FILE__, __LINE__)
void *Hunk_AllocDebug( int size, ha_pref preference, char *label, char *file, int line );
#else
void *Hunk_Alloc( int size, ha_pref preference );
#endif

#define Com_Memset memset
#define Com_Memcpy memcpy

#define CIN_system	1
#define CIN_loop	2
#define	CIN_hold	4
#define CIN_silent	8
#define CIN_shader	16

/*
==============================================================

MATHLIB

==============================================================
*/


typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

typedef	int	fixed4_t;
typedef	int	fixed8_t;
typedef	int	fixed16_t;

#ifndef M_PI
#define M_PI		3.14159265358979323846f	// matches value in gcc v2 math.h
#endif

#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923f	/* pi/2 */
#endif

#define NUMVERTEXNORMALS	162
extern	vec3_t	bytedirs[NUMVERTEXNORMALS];

// all drawing is done to a 640*480 virtual screen size
// and will be automatically scaled to the real resolution
#define	SCREEN_WIDTH		640
#define	SCREEN_HEIGHT		480

#define TINYCHAR_WIDTH		(SMALLCHAR_WIDTH)
#define TINYCHAR_HEIGHT		(SMALLCHAR_HEIGHT/2)

#define SMALLCHAR_WIDTH		8
#define SMALLCHAR_HEIGHT	16

#define BIGCHAR_WIDTH		16
#define BIGCHAR_HEIGHT		16

#define	GIANTCHAR_WIDTH		32
#define	GIANTCHAR_HEIGHT	48

extern	vec4_t		colorBlack;
extern	vec4_t		colorRed;
extern	vec4_t		colorGreen;
extern	vec4_t		colorBlue;
extern	vec4_t		colorYellow;
extern	vec4_t		colorMagenta;
extern	vec4_t		colorCyan;
extern	vec4_t		colorWhite;
extern	vec4_t		colorLtGrey;
extern	vec4_t		colorMdGrey;
extern	vec4_t		colorDkGrey;

#define Q_COLOR_ESCAPE	'^'
#define Q_IsColorString(p)	((p) && *(p) == Q_COLOR_ESCAPE && *((p)+1) && (*((p)+1) >= '0' && *((p)+1) <= '7')) // ^[0-7]

#define COLOR_BLACK	'0'
#define COLOR_RED	'1'
#define COLOR_GREEN	'2'
#define COLOR_YELLOW	'3'
#define COLOR_BLUE	'4'
#define COLOR_CYAN	'5'
#define COLOR_MAGENTA	'6'
#define COLOR_WHITE	'7'
#define ColorIndex(c)	(((c) - '0') & 0x07)

#define S_COLOR_BLACK	"^0"
#define S_COLOR_RED	"^1"
#define S_COLOR_GREEN	"^2"
#define S_COLOR_YELLOW	"^3"
#define S_COLOR_BLUE	"^4"
#define S_COLOR_CYAN	"^5"
#define S_COLOR_MAGENTA	"^6"
#define S_COLOR_WHITE	"^7"

extern vec4_t	g_color_table[8];

#define	MAKERGB( v, r, g, b ) v[0]=r;v[1]=g;v[2]=b
#define	MAKERGBA( v, r, g, b, a ) v[0]=r;v[1]=g;v[2]=b;v[3]=a

#define DEG2RAD( a ) ( ( (a) * M_PI ) / 180.0F )
#define RAD2DEG( a ) ( ( (a) * 180.0f ) / M_PI )

struct cplane_s;

extern	vec3_t	vec3_origin;
extern	vec3_t	axisDefault[3];

#define	nanmask (255<<23)

#define	IS_NAN(x) (((*(int *)&x)&nanmask)==nanmask)

int Q_isnan(float x);

#if idx64
  extern long qftolsse(float f);
  extern int qvmftolsse(void);
  extern void qsnapvectorsse(vec3_t vec);

  #define Q_ftol qftolsse
  #define Q_SnapVector qsnapvectorsse

  extern int (*Q_VMftol)(void);
#elif id386
  extern long QDECL qftolx87(float f);
  extern long QDECL qftolsse(float f);
  extern int QDECL qvmftolx87(void);
  extern int QDECL qvmftolsse(void);
  extern void QDECL qsnapvectorx87(vec3_t vec);
  extern void QDECL qsnapvectorsse(vec3_t vec);

  extern long (QDECL *Q_ftol)(float f);
  extern int (QDECL *Q_VMftol)(void);
  extern void (QDECL *Q_SnapVector)(vec3_t vec);
#else
  // Q_ftol must expand to a function name so the pluggable renderer can take
  // its address
  #define Q_ftol lrintf
  #define Q_SnapVector(vec)\
	do\
	{\
		vec3_t *temp = (vec);\
		\
		(*temp)[0] = round((*temp)[0]);\
		(*temp)[1] = round((*temp)[1]);\
		(*temp)[2] = round((*temp)[2]);\
	} while(0)
#endif
/*
// if your system does not have lrintf() and round() you can try this block. Please also open a bug report at bugzilla.icculus.org
// or write a mail to the ioq3 mailing list.
#else
  #define Q_ftol(v) ((long) (v))
  #define Q_round(v) do { if((v) < 0) (v) -= 0.5f; else (v) += 0.5f; (v) = Q_ftol((v)); } while(0)
  #define Q_SnapVector(vec) \
	do\
	{\
		vec3_t *temp = (vec);\
		\
		Q_round((*temp)[0]);\
		Q_round((*temp)[1]);\
		Q_round((*temp)[2]);\
	} while(0)
#endif
*/

#if idppc

static ID_INLINE float Q_rsqrt( float number ) {
		float x = 0.5f * number;
                float y;
#ifdef __GNUC__            
                asm("frsqrte %0,%1" : "=f" (y) : "f" (number));
#else
		y = __frsqrte( number );
#endif
		return y * (1.5f - (x * y * y));
	}

#ifdef __GNUC__            
static ID_INLINE float Q_fabs(float x) {
    float abs_x;
    
    asm("fabs %0,%1" : "=f" (abs_x) : "f" (x));
    return abs_x;
}
#else
#define Q_fabs __fabsf
#endif

#else
float Q_fabs( float f );
float Q_rsqrt( float f );		// reciprocal square root
#endif

#define SQRTFAST( x ) ( (x) * Q_rsqrt( x ) )

signed char ClampChar( int i );
signed short ClampShort( int i );

// this isn't a real cheap function to call!
int DirToByte( vec3_t dir );
void ByteToDir( int b, vec3_t dir );

#if	1

#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))

#else

#define DotProduct(x,y)			_DotProduct(x,y)
#define VectorSubtract(a,b,c)	_VectorSubtract(a,b,c)
#define VectorAdd(a,b,c)		_VectorAdd(a,b,c)
#define VectorCopy(a,b)			_VectorCopy(a,b)
#define	VectorScale(v, s, o)	_VectorScale(v,s,o)
#define	VectorMA(v, s, b, o)	_VectorMA(v,s,b,o)

#endif

#ifdef Q3_VM
#ifdef VectorCopy
#undef VectorCopy
// this is a little hack to get more efficient copies in our interpreter
typedef struct {
	float	v[3];
} vec3struct_t;
#define VectorCopy(a,b)	(*(vec3struct_t *)b=*(vec3struct_t *)a)
#endif
#endif

#define VectorClear(a)			((a)[0]=(a)[1]=(a)[2]=0)
#define VectorNegate(a,b)		((b)[0]=-(a)[0],(b)[1]=-(a)[1],(b)[2]=-(a)[2])
#define VectorSet(v, x, y, z)	((v)[0]=(x), (v)[1]=(y), (v)[2]=(z))
#define Vector4Copy(a,b)		((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])

#define Byte4Copy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])

#define	SnapVector(v) {v[0]=((int)(v[0]));v[1]=((int)(v[1]));v[2]=((int)(v[2]));}
// just in case you do't want to use the macros
vec_t _DotProduct( const vec3_t v1, const vec3_t v2 );
void _VectorSubtract( const vec3_t veca, const vec3_t vecb, vec3_t out );
void _VectorAdd( const vec3_t veca, const vec3_t vecb, vec3_t out );
void _VectorCopy( const vec3_t in, vec3_t out );
void _VectorScale( const vec3_t in, float scale, vec3_t out );
void _VectorMA( const vec3_t veca, float scale, const vec3_t vecb, vec3_t vecc );

unsigned ColorBytes3 (float r, float g, float b);
unsigned ColorBytes4 (float r, float g, float b, float a);

float NormalizeColor( const vec3_t in, vec3_t out );

float RadiusFromBounds( const vec3_t mins, const vec3_t maxs );
void ClearBounds( vec3_t mins, vec3_t maxs );
void AddPointToBounds( const vec3_t v, vec3_t mins, vec3_t maxs );

#if !defined( Q3_VM ) || ( defined( Q3_VM ) && defined( __Q3_VM_MATH ) )
static ID_INLINE int VectorCompare( const vec3_t v1, const vec3_t v2 ) {
	if (v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2]) {
		return 0;
	}			
	return 1;
}

static ID_INLINE int VectorCompareEpsilon(
		const vec3_t v1, const vec3_t v2, float epsilon )
{
	vec3_t d;

	VectorSubtract( v1, v2, d );
	d[ 0 ] = fabs( d[ 0 ] );
	d[ 1 ] = fabs( d[ 1 ] );
	d[ 2 ] = fabs( d[ 2 ] );

	if( d[ 0 ] > epsilon || d[ 1 ] > epsilon || d[ 2 ] > epsilon )
		return 0;

	return 1;
}

static ID_INLINE vec_t VectorLength( const vec3_t v ) {
	return (vec_t)sqrt (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

static ID_INLINE vec_t VectorLengthSquared( const vec3_t v ) {
	return (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

static ID_INLINE vec_t Distance( const vec3_t p1, const vec3_t p2 ) {
	vec3_t	v;

	VectorSubtract (p2, p1, v);
	return VectorLength( v );
}

static ID_INLINE vec_t DistanceSquared( const vec3_t p1, const vec3_t p2 ) {
	vec3_t	v;

	VectorSubtract (p2, p1, v);
	return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}

// fast vector normalize routine that does not check to make sure
// that length != 0, nor does it return length, uses rsqrt approximation
static ID_INLINE void VectorNormalizeFast( vec3_t v )
{
	float ilength;

	ilength = Q_rsqrt( DotProduct( v, v ) );

	v[0] *= ilength;
	v[1] *= ilength;
	v[2] *= ilength;
}

static ID_INLINE void VectorInverse( vec3_t v ){
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

static ID_INLINE void CrossProduct( const vec3_t v1, const vec3_t v2, vec3_t cross ) {
	cross[0] = v1[1]*v2[2] - v1[2]*v2[1];
	cross[1] = v1[2]*v2[0] - v1[0]*v2[2];
	cross[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

static ID_INLINE int FloatAsInt( float f ) {
	floatint_t fi;
	fi.f = f;
	return fi.i;
}

static ID_INLINE float IntAsFloat( int i ) {
	floatint_t fi;
	fi.i = i;
	return fi.f;
}

#else
int VectorCompare( const vec3_t v1, const vec3_t v2 );

// VectorCompareEpsilon

vec_t VectorLength( const vec3_t v );

vec_t VectorLengthSquared( const vec3_t v );

vec_t Distance( const vec3_t p1, const vec3_t p2 );

vec_t DistanceSquared( const vec3_t p1, const vec3_t p2 );

void VectorNormalizeFast( vec3_t v );

void VectorInverse( vec3_t v );

void CrossProduct( const vec3_t v1, const vec3_t v2, vec3_t cross );

int FloatAsInt( float f );

float IntAsFloat( int i );
#endif

vec_t VectorNormalize (vec3_t v);		// returns vector length
vec_t VectorNormalize2( const vec3_t v, vec3_t out );
void Vector4Scale( const vec4_t in, vec_t scale, vec4_t out );
void VectorRotate( vec3_t in, vec3_t matrix[3], vec3_t out );
int Q_log2(int val);

int		Q_rand( int *seed );
float	Q_random( int *seed );
float	Q_crandom( int *seed );

#define random()	((rand () & 0x7fff) / ((float)0x7fff))
#define crandom()	(2.0 * (random() - 0.5))

void vectoangles( const vec3_t value1, vec3_t angles);
void AnglesToAxis( const vec3_t angles, vec3_t axis[3] );

void AxisClear( vec3_t axis[3] );
void AxisCopy( vec3_t in[3], vec3_t out[3] );

void SetPlaneSignbits( struct cplane_s *out );
int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, struct cplane_s *plane);

qboolean BoundsIntersect(const vec3_t mins, const vec3_t maxs,
		const vec3_t mins2, const vec3_t maxs2);
qboolean BoundsIntersectSphere(const vec3_t mins, const vec3_t maxs,
		const vec3_t origin, vec_t radius);
qboolean BoundsIntersectPoint(const vec3_t mins, const vec3_t maxs,
		const vec3_t origin);

float	AngleMod(float a);
float	LerpAngle (float from, float to, float frac);
float	AngleSubtract( float a1, float a2 );
void	AnglesSubtract( vec3_t v1, vec3_t v2, vec3_t v3 );

float AngleNormalize360 ( float angle );
float AngleNormalize180 ( float angle );
float AngleDelta ( float angle1, float angle2 );

qboolean PlaneFromPoints( vec4_t plane, const vec3_t a, const vec3_t b, const vec3_t c );
void ProjectPointOnPlane( vec3_t dst, const vec3_t p, const vec3_t normal );
void RotatePointAroundVector( vec3_t dst, const vec3_t dir, const vec3_t point, float degrees );
void RotateAroundDirection( vec3_t axis[3], float yaw );
float Q_acos(float c);
float Q_asin(float c);
void MakeNormalVectors( const vec3_t forward, vec3_t right, vec3_t up );
// perpendicular vector could be replaced by this

//int	PlaneTypeForNormal (vec3_t normal);

void MatrixMultiply(float in1[3][3], float in2[3][3], float out[3][3]);
void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
void PerpendicularVector( vec3_t dst, const vec3_t src );

vec_t DistanceBetweenLineSegmentsSquared(
	const vec3_t sP0, const vec3_t sP1,
	const vec3_t tP0, const vec3_t tP1,
	float *s, float *t );

#ifndef MAX
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif

#ifndef MIN
#define MIN(x,y) ((x)<(y)?(x):(y))
#endif

//=============================================

float Com_Clamp( float min, float max, float value );

char	*COM_SkipPath( char *pathname );
const char	*COM_GetExtension( const char *name );
void	COM_StripExtension(const char *in, char *out, int destsize);
qboolean COM_CompareExtension(const char *in, const char *ext);
void	COM_DefaultExtension( char *path, int maxSize, const char *extension );
void	COM_SetExtension(char *path, int maxSize, const char *extension);

void	COM_BeginParseSession( const char *name );
int		COM_GetCurrentParseLine( void );
char	*COM_Parse( char **data_p );
char	*COM_ParseExt( char **data_p, qboolean allowLineBreaks );
char	*COM_ParseExt2( char **data_p, qboolean allowLineBreaks, char delimiter );
int		COM_Compress( char *data_p );
void	COM_ParseError( char *format, ... ) __attribute__ ((format (printf, 1, 2)));
void	COM_ParseWarning( char *format, ... ) __attribute__ ((format (printf, 1, 2)));
//int		COM_ParseInfos( char *buf, int max, char infos[][MAX_INFO_STRING] );

#define MAX_TOKENLENGTH		1024

#ifndef TT_STRING
//token types
#define TT_STRING					1			// string
#define TT_LITERAL					2			// literal
#define TT_NUMBER					3			// number
#define TT_NAME						4			// name
#define TT_PUNCTUATION				5			// punctuation

//string sub type
//---------------
//		the length of the string
//literal sub type
//----------------
//		the ASCII code of the literal
//number sub type
//---------------
#define TT_DECIMAL					0x0008	// decimal number
#define TT_HEX							0x0100	// hexadecimal number
#define TT_OCTAL						0x0200	// octal number
//#ifdef BINARYNUMBERS
#define TT_BINARY						0x0400	// binary number
//#endif //BINARYNUMBERS
#define TT_FLOAT						0x0800	// floating point number
#define TT_INTEGER					0x1000	// integer number
#define TT_LONG						0x2000	// long number
#define TT_UNSIGNED					0x4000	// unsigned number
//punctuation sub type
//--------------------
#define P_RSHIFT_ASSIGN				1
#define P_LSHIFT_ASSIGN				2
#define P_PARMS						3
#define P_PRECOMPMERGE				4

#define P_LOGIC_AND					5
#define P_LOGIC_OR					6
#define P_LOGIC_GEQ					7
#define P_LOGIC_LEQ					8
#define P_LOGIC_EQ					9
#define P_LOGIC_UNEQ					10

#define P_MUL_ASSIGN					11
#define P_DIV_ASSIGN					12
#define P_MOD_ASSIGN					13
#define P_ADD_ASSIGN					14
#define P_SUB_ASSIGN					15
#define P_INC							16
#define P_DEC							17

#define P_BIN_AND_ASSIGN			18
#define P_BIN_OR_ASSIGN				19
#define P_BIN_XOR_ASSIGN			20
#define P_RSHIFT						21
#define P_LSHIFT						22

#define P_POINTERREF					23
#define P_CPP1							24
#define P_CPP2							25
#define P_MUL							26
#define P_DIV							27
#define P_MOD							28
#define P_ADD							29
#define P_SUB							30
#define P_ASSIGN						31

#define P_BIN_AND						32
#define P_BIN_OR						33
#define P_BIN_XOR						34
#define P_BIN_NOT						35

#define P_LOGIC_NOT					36
#define P_LOGIC_GREATER				37
#define P_LOGIC_LESS					38

#define P_REF							39
#define P_COMMA						40
#define P_SEMICOLON					41
#define P_COLON						42
#define P_QUESTIONMARK				43

#define P_PARENTHESESOPEN			44
#define P_PARENTHESESCLOSE			45
#define P_BRACEOPEN					46
#define P_BRACECLOSE					47
#define P_SQBRACKETOPEN				48
#define P_SQBRACKETCLOSE			49
#define P_BACKSLASH					50

#define P_PRECOMP						51
#define P_DOLLAR						52
//name sub type
//-------------
//		the length of the name
#endif

typedef struct pc_token_s
{
	int type;
	int subtype;
	int intvalue;
	float floatvalue;
	char string[MAX_TOKENLENGTH];
} pc_token_t;

// data is an in/out parm, returns a parsed out token

void	COM_MatchToken( char**buf_p, char *match );

qboolean SkipBracedSection (char **program, int depth);
void SkipRestOfLine ( char **data );

void Parse1DMatrix (char **buf_p, int x, float *m);
void Parse2DMatrix (char **buf_p, int y, int x, float *m);
void Parse3DMatrix (char **buf_p, int z, int y, int x, float *m);
int Com_HexStrToInt( const char *str );

int QDECL Com_sprintf (char *dest, int size, const char *fmt, ...) __attribute__ ((format (printf, 3, 4)));

char *Com_SkipTokens( char *s, int numTokens, char *sep );
char *Com_SkipCharset( char *s, char *sep );

void Com_RandomBytes( byte *string, int len );

typedef struct 
{
	unsigned int hi;
	unsigned int lo;
} clientList_t;

qboolean Com_ClientListContains( const clientList_t *list, int clientNum );
void Com_ClientListAdd( clientList_t *list, int clientNum );
void Com_ClientListRemove( clientList_t *list, int clientNum );
char *Com_ClientListString( const clientList_t *list );
void Com_ClientListParse( clientList_t *list, const char *s );

// mode parm for FS_FOpenFile
typedef enum {
	FS_READ,
	FS_WRITE,
	FS_APPEND,
	FS_APPEND_SYNC
} fsMode_t;

typedef enum {
	FS_SEEK_CUR,
	FS_SEEK_END,
	FS_SEEK_SET
} fsOrigin_t;

//=============================================

int Q_isprint( int c );
int Q_islower( int c );
int Q_isupper( int c );
int Q_isalpha( int c );
qboolean Q_isanumber( const char *s );
qboolean Q_isintegral( float f );

// portable case insensitive compare
int		Q_stricmp (const char *s1, const char *s2);
int		Q_strncmp (const char *s1, const char *s2, int n);
int		Q_stricmpn (const char *s1, const char *s2, int n);
char	*Q_strlwr( char *s1 );
char	*Q_strupr( char *s1 );
const char	*Q_stristr( const char *s, const char *find);

// buffer size safe library replacements
void	Q_strncpyz( char *dest, const char *src, int destsize );
void	Q_strcat( char *dest, int size, const char *src );

// strlen that discounts Quake color sequences
int Q_PrintStrlen( const char *string );
// removes color sequences from string
char *Q_CleanStr( char *string );
// Count the number of char tocount encountered in string
int Q_CountChar(const char *string, char tocount);

//=============================================

// 64-bit integers for global rankings interface
// implemented as a struct for qvm compatibility
typedef struct
{
	byte	b0;
	byte	b1;
	byte	b2;
	byte	b3;
	byte	b4;
	byte	b5;
	byte	b6;
	byte	b7;
} qint64;

//=============================================
/*
short	BigShort(short l);
short	LittleShort(short l);
int		BigLong (int l);
int		LittleLong (int l);
qint64  BigLong64 (qint64 l);
qint64  LittleLong64 (qint64 l);
float	BigFloat (const float *l);
float	LittleFloat (const float *l);

void	Swap_Init (void);
*/
char	* QDECL va(char *format, ...) __attribute__ ((format (printf, 1, 2)));

#define TRUNCATE_LENGTH	64
void Com_TruncateLongString( char *buffer, const char *s );

//=============================================

//
// key / value info strings
//
char *Info_ValueForKey( const char *s, const char *key );
void Info_RemoveKey( char *s, const char *key );
void Info_RemoveKey_Big( char *s, const char *key );
void Info_SetValueForKey( char *s, const char *key, const char *value );
void Info_SetValueForKey_Big( char *s, const char *key, const char *value );
qboolean Info_Validate( const char *s );
void Info_NextPair( const char **s, char *key, char *value );

// this is only here so the functions in q_shared.c and bg_*.c can link
void	QDECL Com_Error( int level, const char *error, ... ) __attribute__ ((noreturn, format(printf, 2, 3)));
void	QDECL Com_Printf( const char *msg, ... ) __attribute__ ((format (printf, 1, 2)));
void	QDECL Com_DPrintf( const char *msg, ... ) __attribute__ ((format (printf, 1, 2)));


/*
==========================================================

CVARS (console variables)

Many variables can be used for cheating purposes, so when
cheats is zero, force all unspecified variables to their
default values.
==========================================================
*/

#define	CVAR_ARCHIVE		0x0001	// set to cause it to be saved to vars.rc
					// used for system variables, not for player
					// specific configurations
#define	CVAR_USERINFO		0x0002	// sent to server on connect or change
#define	CVAR_SERVERINFO		0x0004	// sent in response to front end requests
#define	CVAR_SYSTEMINFO		0x0008	// these cvars will be duplicated on all clients
#define	CVAR_INIT		0x0010	// don't allow change from console at all,
					// but can be set from the command line
#define	CVAR_LATCH		0x0020	// will only change when C code next does
					// a Cvar_Get(), so it can't be changed
					// without proper initialization.  modified
					// will be set, even though the value hasn't
					// changed yet
#define	CVAR_ROM		0x0040	// display only, cannot be set by user at all
#define	CVAR_USER_CREATED	0x0080	// created by a set command
#define	CVAR_TEMP		0x0100	// can be set even when cheats are disabled, but is not archived
#define CVAR_CHEAT		0x0200	// can not be changed if cheats are disabled
#define CVAR_NORESTART		0x0400	// do not clear when a cvar_restart is issued

#define CVAR_SERVER_CREATED	0x0800	// cvar was created by a server the client connected to.
#define CVAR_VM_CREATED		0x1000	// cvar was created exclusively in one of the VMs.
#define CVAR_PROTECTED		0x2000	// prevent modifying this var from VMs or the server
#define CVAR_USERINFO2		0x4000 // userinfo for second local player
#define CVAR_USERINFO3		0x8000 // userinfo for third local player
#define CVAR_USERINFO4		0x10000 // userinfo for fourth local player
// These flags are only returned by the Cvar_Flags() function
#define CVAR_MODIFIED		0x40000000	// Cvar was modified
#define CVAR_NONEXISTENT	0x80000000	// Cvar doesn't exist.

#define CVAR_USERINFO_ALL	(CVAR_USERINFO|CVAR_USERINFO2|CVAR_USERINFO3|CVAR_USERINFO4)

// nothing outside the Cvar_*() functions should modify these fields!
typedef struct cvar_s cvar_t;

struct cvar_s {
	char			*name;
	char			*string;
	char			*resetString;		// cvar_restart will reset to this value
	char			*latchedString;		// for CVAR_LATCH vars
	int				flags;
	qboolean	modified;			// set each time the cvar is changed
	int				modificationCount;	// incremented each time the cvar is changed
	float			value;				// atof( string )
	int				integer;			// atoi( string )
	qboolean	validate;
	qboolean	integral;
	float			min;
	float			max;

	cvar_t *next;
	cvar_t *prev;
	cvar_t *hashNext;
	cvar_t *hashPrev;
	int			hashIndex;
};

#define	MAX_CVAR_VALUE_STRING	256

typedef int	cvarHandle_t;

// the modules that run in the virtual machine can't access the cvar_t directly,
// so they must ask for structured updates
typedef struct {
	cvarHandle_t	handle;
	int			modificationCount;
	float		value;
	int			integer;
	char		string[MAX_CVAR_VALUE_STRING];
} vmCvar_t;


/*
==============================================================

VoIP

==============================================================
*/

// if you change the count of flags be sure to also change VOIP_FLAGNUM
#define VOIP_SPATIAL		0x01		// spatialized voip message
#define VOIP_DIRECT		0x02		// non-spatialized voip message

// number of flags voip knows. You will have to bump protocol version number if you
// change this.
#define VOIP_FLAGCNT		2

/*
==============================================================

COLLISION DETECTION

==============================================================
*/

#include "surfaceflags.h"			// shared with the q3map utility

// plane types are used to speed some tests
// 0-2 are axial planes
#define	PLANE_X				0
#define	PLANE_Y				1
#define	PLANE_Z				2
#define	PLANE_NON_AXIAL		3
#define PLANE_NON_PLANAR	4


/*
=================
PlaneTypeForNormal
=================
*/

#define PlaneTypeForNormal( x ) ( x[0] == 1.0 ? PLANE_X : ( x[1] == 1.0 ? PLANE_Y : ( x[2] == 1.0 ? PLANE_Z : ( x[0] == 0.f && x[1] == 0.f && x[2] == 0.f ? PLANE_NON_PLANAR : PLANE_NON_AXIAL ) ) ) )

// plane_t structure
// !!! if this is changed, it must be changed in asm code too !!!
typedef struct cplane_s {
	vec3_t	normal;
	float	dist;
	byte	type;			// for fast side tests: 0,1,2 = axial, 3 = nonaxial
	byte	signbits;		// signx + (signy<<1) + (signz<<2), used as lookup during collision
	byte	pad[2];
} cplane_t;

typedef enum {
	TT_NONE,

	TT_AABB,
	TT_CAPSULE,
	TT_BISPHERE,

	TT_NUM_TRACE_TYPES
} traceType_t;

// a trace is returned when a box is swept through the world
typedef struct {
	qboolean	allsolid;	// if true, plane is not valid
	qboolean	startsolid;	// if true, the initial point was in a solid area
	float		fraction;	// time completed, 1.0 = didn't hit anything
	vec3_t		endpos;		// final position
	cplane_t	plane;		// surface normal at impact, transformed to world space
	int			surfaceNum;		// the contacted surface number plus one
	int			surfaceFlags;	// surface hit
	int			contents;	// contents on other side of surface hit
	int			entityNum;	// entity the contacted sirface is a part of
	float		lateralFraction; // fraction of collision tangetially to the trace direction
} trace_t;

// trace->entityNum can also be 0 to (MAX_GENTITIES-1)
// or ENTITYNUM_NONE, ENTITYNUM_WORLD


// markfragments are returned by R_MarkFragments()
typedef struct {
	int		firstPoint;
	int		numPoints;
} markFragment_t;



typedef struct {
	vec3_t		origin;
	vec3_t		axis[3];
} orientation_t;

//=====================================================================


// in order from highest priority to lowest
// if none of the catchers are active, bound key strings will be executed
#define KEYCATCH_CONSOLE		0x0001
#define	KEYCATCH_UI					0x0002
#define	KEYCATCH_MESSAGE		0x0004
#define	KEYCATCH_CGAME			0x0008

#define KEYCATCH_UI_CGAME		( KEYCATCH_UI | KEYCATCH_CGAME | KEYCATCH_MESSAGE | KEYCATCH_CONSOLE )


// sound channels
// channel 0 never willingly overrides
// other channels will allways override a playing sound on that channel
typedef enum {
	CHAN_AUTO,
	CHAN_LOCAL,		// menu sounds, etc
	CHAN_WEAPON,
	CHAN_VOICE,
	CHAN_ITEM,
	CHAN_BODY,
	CHAN_LOCAL_SOUND,	// chat messages, etc
	CHAN_ANNOUNCER		// announcer voices, etc
} soundChannel_t;


/*
========================================================================

  ELEMENTS COMMUNICATED ACROSS THE NET

========================================================================
*/

#define	ANGLE2SHORT(x)	((int)((x)*65536/360) & 65535)
#define	SHORT2ANGLE(x)	((x)*(360.0/65536))

#define	SNAPFLAG_RATE_DELAYED	1
#define	SNAPFLAG_NOT_ACTIVE		2	// snapshot used during connection and for zombies
#define SNAPFLAG_SERVERCOUNT	4	// toggled every map_restart so transitions can be detected

//
// per-level limits
//
#define	MAX_CLIENTS			64		// absolute limit

#define	GENTITYNUM_BITS		12		// don't need to send any more
#define	MAX_GENTITIES		(1<<GENTITYNUM_BITS)

// entitynums are communicated with GENTITY_BITS, so any reserved
// values that are going to be communcated over the net need to
// also be in this range
#define	ENTITYNUM_NONE		(MAX_GENTITIES-1)
#define	ENTITYNUM_WORLD		(MAX_GENTITIES-2)
#define	ENTITYNUM_MAX_NORMAL	(MAX_GENTITIES-2)

#define	MAX_CONFIGSTRINGS	1024

// these are the only configstrings that the system reserves, all the
// other ones are strictly for servergame to clientgame communication
#define	CS_SERVERINFO		0		// an info string with all the serverinfo cvars
#define	CS_SYSTEMINFO		1		// an info string for server system to client system configuration (timescale, etc)

#define	RESERVED_CONFIGSTRINGS	2	// game can't modify below this, only the system can

#define	MAX_GAMESTATE_CHARS	16000
typedef struct {
	int			stringOffsets[MAX_CONFIGSTRINGS];
	char		stringData[MAX_GAMESTATE_CHARS];
	int			dataCount;
} gameState_t;

//=========================================================

// New fields cannot be added to sharedPlayerState_t without updating
//   playerState_t in bg_misc.h (and thus breaking mod compatiblity).

typedef struct sharedPlayerState_s {
	int			commandTime;	// cmd->serverTime of last executed command

	vec3_t		origin;

	qboolean	linked;			// set by server

	int			clientNum;		// ranges from 0 to MAX_CLIENTS-1

	vec3_t		viewangles;		// for fixed views
	int			viewheight;

	int			ping;			// server to game info for scoreboard

	// ZTM: FIXME: make persistant private to game/cgame. Currently server accesses PERS_SCORE (0) in it.
	int			persistant[1];	// stats that aren't cleared on death

} sharedPlayerState_t;


// Max players for splitscreen
// Also see CL_MAX_SPLITVIEW in client.h for lowering max supported splitview
// clients in client/renderer, while still keep network and mod compatibility.
#define MAX_SPLITVIEW 4

//====================================================================

// usercmd_t is sent to the server each client frame
typedef struct usercmd_s {
	int				serverTime;
	int				angles[3];
	int 			buttons;
	int				stateValue;
	signed char	forwardmove, rightmove, upmove;
} usercmd_t;

//===================================================================

// New fields cannot be added to sharedEntityState_t without updating
//   entityState_t in bg_misc.h (and thus breaking mod compatiblity).

typedef struct sharedEntityState_s {
	int		number;			// entity index

	int		contents;		// CONTENTS_TRIGGER, CONTENTS_SOLID, CONTENTS_BODY, etc
							// a non-solid entity should set to 0

	qboolean	capsule;	// if true, use capsule instead of bbox for clipping against this ent

	qboolean	bmodel;		// if true, modelindex is an inline model number
							// if false, assume an explicit mins / maxs bounding box
							// only set by trap_SetBrushModel

	int		modelindex;

	vec3_t		mins, maxs;	// bounding box size

	vec3_t	origin;
	vec3_t	origin2;

} sharedEntityState_t;

typedef struct {
	int		offset;
	int		numElements; // 1 to 1024
	int		bits;		// 0 = float
} vmNetField_t;

typedef enum {
	CA_UNINITIALIZED,
	CA_DISCONNECTED, 	// not talking to a server
	CA_CONNECTING,		// sending request packets to the server
	CA_CHALLENGING,		// sending challenge packets to the server
	CA_CONNECTED,		// netchan_t established, getting gamestate
	CA_LOADING,			// only during cgame initialization, never during main loop
	CA_PRIMED,			// got gamestate, waiting for first frame
	CA_ACTIVE,			// game views should be displayed
	CA_CINEMATIC		// playing a cinematic or a static pic, not connected to a server
} connstate_t;

// font support 

#define GLYPH_START 0
#define GLYPH_END 255
#define GLYPH_CHARSTART 32
#define GLYPH_CHAREND 127
#define GLYPHS_PER_FONT GLYPH_END - GLYPH_START + 1
typedef struct {
  int height;       // number of scan lines
  int top;          // top of glyph in buffer
  int left;         // left of glyph in buffer
  int pitch;        // width for copying
  int xSkip;        // x adjustment
  int imageWidth;   // width of actual image
  int imageHeight;  // height of actual image
  float s;          // x offset in image where glyph starts
  float t;          // y offset in image where glyph starts
  float s2;
  float t2;
  qhandle_t glyph;  // handle to the shader with the glyph
  char shaderName[32];
} glyphInfo_t;

typedef struct {
  glyphInfo_t glyphs [GLYPHS_PER_FONT];
  float glyphScale;
  char name[MAX_QPATH];
} fontInfo_t;

char *Com_LocalClientCvarName(int localClient, const char *in_cvarName);
int Com_LocalClientForCvarName(const char *in_cvarName);
const char *Com_LocalClientBaseCvarName(const char *in_cvarName);

#define Square(x) ((x)*(x))

// real time
//=============================================


typedef struct qtime_s {
	int tm_sec;     /* seconds after the minute - [0,59] */
	int tm_min;     /* minutes after the hour - [0,59] */
	int tm_hour;    /* hours since midnight - [0,23] */
	int tm_mday;    /* day of the month - [1,31] */
	int tm_mon;     /* months since January - [0,11] */
	int tm_year;    /* years since 1900 */
	int tm_wday;    /* days since Sunday - [0,6] */
	int tm_yday;    /* days since January 1 - [0,365] */
	int tm_isdst;   /* daylight savings time flag */
} qtime_t;


// cinematic states
typedef enum {
	FMV_IDLE,
	FMV_PLAY,		// play
	FMV_EOF,		// all other conditions, i.e. stop/EOF/abort
	FMV_ID_BLT,
	FMV_ID_IDLE,
	FMV_LOOPED,
	FMV_ID_WAIT
} e_status;


#define MAX_JOYSTICK_AXIS 16



#define	MAX_GLOBAL_SERVERS				4096
#define	MAX_OTHER_SERVERS					128
#define MAX_PINGREQUESTS					32
#define MAX_SERVERSTATUSREQUESTS	16

#define SAY_ALL		0
#define SAY_TEAM	1
#define SAY_TELL	2

#define LERP( a, b, w ) ( ( a ) * ( 1.0f - ( w ) ) + ( b ) * ( w ) )
#define LUMA( red, green, blue ) ( 0.2126f * ( red ) + 0.7152f * ( green ) + 0.0722f * ( blue ) )

#endif	// __Q_SHARED_H
