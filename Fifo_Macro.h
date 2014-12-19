// create a macro pointer Fifo implementation

#ifndef __FIFO_MACRO__
#define __FIFO_MACRO__

#define CreateFifo(FIFONAME, SIZE, DATATYPE, SUCCESS, FAIL)		\
DATATYPE volatile *FIFONAME ## PutPt; 												\
DATATYPE volatile *FIFONAME ## GetPt;													\
DATATYPE static   FIFONAME ## Fifo[SIZE];											\
void FIFONAME ## Fifo_Init(void)															\
{																															\
	FIFONAME ## PutPt= FIFONAME ## GetPt= &FIFONAME ## Fifo[0];	\
}																															\
int FIFONAME ## Fifo_Put(DATATYPE newElement)									\
{																															\
	DATATYPE volatile *nextPutPt;																\
	nextPutPt= FIFONAME ## PutPt + 1;		        								\
																															\
	if (nextPutPt == &(FIFONAME ## Fifo[SIZE]))									\
	{																														\
		nextPutPt= &FIFONAME ## Fifo[0];													\
	}																														\
	if (nextPutPt == (FIFONAME ## GetPt))												\
	{																														\
		return (FAIL);																						\
	}																														\
  *(FIFONAME ## PutPt)= newElement;														\
	 (FIFONAME ## PutPt)= nextPutPt;														\
	return (SUCCESS);																						\
}																															\
int FIFONAME ## Fifo_Get(DATATYPE *firstElement)							\
{																															\
	if ((FIFONAME ## GetPt) == (FIFONAME ## PutPt))							\
	{																														\
		return (FAIL);																						\
	}																														\
	*firstElement= *(FIFONAME ## GetPt);												\
	++(FIFONAME ## GetPt);																			\
	if ((FIFONAME ## GetPt) == &(FIFONAME ## Fifo[SIZE]))	 			\
	{																														\
		(FIFONAME ## GetPt)= &(FIFONAME ## Fifo[0]);				 			\
	}																														\
	return (SUCCESS);																						\
}																															

#endif
