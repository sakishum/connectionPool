/**
 * @file assist.h
 * @Synopsis  Assist in debugging
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2013-02-21
 */

#ifndef __ASSIST_H__
#define __ASSIST_H__

///=======================宏marco===========================
/// 定义 DEBUG 开关
#ifndef DEBUG
#define DEBUG 1
#endif

/// 定义DEBUG 等级
#define BASIC_DEBUG	1
#define EXTRA_DEBUG	2
#define SUPER_DEBUG	4

/// 定义LOG 函数的宏
#if DEBUG
#define TP_LOG( format, ... ) Tp_write_to_debug_log( format, ##  __VA_ARGS__ )
#define TP_LOG_PTR Tp_write_to_debug_log
#else
#define TP_LOG( format, ... ) do {} while (0)
#define TP_LOG_PTR NULL
#endif

/// 定义extern "C" 的宏
#ifdef __cplusplus
#define DECLS_BEGIN extern "C" {
#define DECLS_END   }
#else
/*The lines within extern "C" */
#define DECLS_BEGIN
#define DECLS_END
#endif/*__cplusplus*/

#if DEBUG
/// 无返回值的函数
#define TP_RETURN_IF_FAILED(p) if(!(p)) \
	{TP_LOG("%s:%d Warning: "#p" failed.\n", \
			__func__, __LINE__); return;}

/// 有返回值的函数
#define TP_RETURN_VAL_IFFAILED(p, ret) if(!(p)) \
	{TP_LOG("%s:%d Warning: "#p" failed.\n", \
			__func__, __LINE__); return (ret);}
#else
#define TP_RETURN_IF_FAILED(p)			do {} while (0)
#define TP_RETURN_VAL_IFFAILED(p, ret)	do {} while (0)
#endif

/// 内存管理的宏
// 01/03/2013 修改了 free 的宏 
// 用法：
//	TP_SAFE_FREE(p);
//	TP_SAFE_DELETE(p);
//	TP_SAFE_DELETEARRAY(p);
//	TP_BREAK_IF(cond);
#define TP_SAFE_FREE(p)			do { if(p) { free(p); (p) = 0; } } while(0) 
#define TP_SAFE_DELETE(p)		do { if(p) { delete (p); (p) = 0; } } while(0)
#define TP_SAFE_DELETEARRAY(P)	do { if(p) { delete[] (p); (p) = 0; } } while(0)
#define TP_BREAK_IF(cond)		if(cond) break

/// 求数组元素个数的宏
#define TP_SIZE_LEN(a) (sizeof(a)/sizeof(a[0])) 

///=======================枚举emum==========================
/// 基本返回值枚举
typedef enum _Ret {
    RET_OK,				/// okey
    RET_OOM,			/// Out Of Memory
    RET_STOP,			/// stop
    RET_INVALID_PARAMS,	/// invalid params
    RET_FAIL			/// fail
} Ret;

///=======================函数global function===============
/// 全局函数声明
void Tp_write_to_debug_log(const char *format, ...);

#endif /*__ASSIST_H__*/
