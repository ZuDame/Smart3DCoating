//===================================================================================================
// Summary:
//		程序内部使用宏的定义
// Date:
//		2011-08-09
// Author:
//		Liuguoqing(gliu@bplead.com)
//===================================================================================================

#ifndef	__BaseMacro_h__
#define __BaseMacro_h__

//===================================================================================================

// 删除选择对象
#define SAFE_DELETE_SELECTION(p) \
{ \
	if (NULL != p) \
	ProSelectionFree(&p); \
	p = NULL; \
}

// 删除选择对象
#define SAFE_DELETE_PROARRAY(p) \
{ \
	if (NULL != p) \
	ProArrayFree((ProArray*)&p); \
	p = NULL; \
}

//===================================================================================================

#define DFE_DELTA 0.001
#define DFE_ZERO 0.000001
#define DEQUAL(x, y) (fabs((x) - (y)) <= DFE_ZERO)
#define ISNEAR(x, y) (fabs((x) - (y)) <= DFE_DELTA)

// 圆周率
#define DOCTOR_PI 3.14159265359
// 角度转弧度
#define ANGLE_TO_RADIAN(angle) (angle) * (0.017453292519943295769236907684883)
#define RADIAN_TO_ANGLE(rad) (rad) * (57.295779513082320876798154814105)

#define MAX_SELECTION	99

//===================================================================================================

#endif