#ifndef SENSORQUEUE_H
#define SENSORQUEUE_H

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "Lock.h"

#define MAX_QUEUE_SIZE	1024		/* queue size */

typedef struct _SensorData
{
	char direction;			/* ... */
	float pitch;			/* ... */
	float roll;				/* ... */
	float yaw;				/* ... */
}SensorData;

class SensorQueue
{
public:

	static SensorQueue* Instance();

	void Enqueue(const SensorData x);

	SensorData* Dequeue();

	int GetQueueItemNum();

	int IsEmpty();

protected:

	SensorQueue();

private:

	SensorData SensorDataQueue[MAX_QUEUE_SIZE];					  /* body of queue */

	int first;													  /* position of first element */

	int last;													  /* position of last element */

	int count;													  /* number of queue elements */

	int sizeofQueue;											  /* size of queue */

	OSLock m_lock;												  /* os lock */

	static SensorQueue* _instance;  /* A static pointer of Class WaveProcessorQueue used for calling back */
};

#endif /* SENSORQUEUE_H */

