#ifndef SERIALQUEUE_H
#define SERIALQUEUE_H

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "Lock.h"

#define MAX_SERIAL_QUEUE_SIZE	128		/* queue size */

typedef struct _SerialData
{
	char content[2048];
	int length;
}SerialData;

class SerialQueue
{
public:

	static SerialQueue* Instance();

	void Enqueue(const SerialData x);

	SerialData* Dequeue();

	int GetQueueItemNum();

	int IsEmpty();

protected:

	SerialQueue();

private:

	SerialData SerialDataQueue[MAX_SERIAL_QUEUE_SIZE];					  /* body of queue */

	int first;													  /* position of first element */

	int last;													  /* position of last element */

	int count;													  /* number of queue elements */

	int sizeofQueue;											  /* size of queue */

	OSLock m_lock;												  /* os lock */

	static SerialQueue* _instance;  /* A static pointer of Class WaveProcessorQueue used for calling back */
};

#endif /*SERIALQUEUE_H */

