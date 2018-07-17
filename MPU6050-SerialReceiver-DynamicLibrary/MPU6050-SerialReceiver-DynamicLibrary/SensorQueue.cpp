#include "stdafx.h"
#include "SensorQueue.h"

SensorQueue* SensorQueue::_instance = NULL;

SensorQueue* SensorQueue::Instance()
{
	if (_instance == 0)
	{
		_instance = new SensorQueue();
	}
	return _instance;
}

SensorQueue::SensorQueue()
{
	first = 0;
	sizeofQueue = MAX_QUEUE_SIZE;
	last = sizeofQueue - 1;
	count = 0;
}

int SensorQueue::IsEmpty()
{
	OSLock::LockOwner lock(m_lock);
	if (count <= 0) return (1);
	else return (0);
}


void SensorQueue::Enqueue(const SensorData x)
{
	OSLock::LockOwner lock(m_lock);
	if (count >= sizeofQueue)
	{
		if (count <= 0)
		{
			;
		}
		else
		{
			first = (first + 1) % sizeofQueue;
			count = count - 1;
		}
	}
	last = (last + 1) % sizeofQueue;
	SensorDataQueue[last] = x;
	count = count + 1;
}

SensorData* SensorQueue::Dequeue()
{
	OSLock::LockOwner lock(m_lock);
	SensorData *result;
	if (count <= 0)
	{
		return NULL;
	}
	else
	{
		result = &SensorDataQueue[first];
		first = (first + 1) % sizeofQueue;
		count = count - 1;
	}
	return(result);
}


int SensorQueue::GetQueueItemNum()
{
	OSLock::LockOwner lock(m_lock);
	return count;
}

