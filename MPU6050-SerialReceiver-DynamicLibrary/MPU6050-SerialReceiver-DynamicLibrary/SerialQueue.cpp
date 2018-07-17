#include "stdafx.h"
#include "SerialQueue.h"

SerialQueue* SerialQueue::_instance = NULL;

SerialQueue* SerialQueue::Instance()
{
	if (_instance == 0)
	{
		_instance = new SerialQueue();
	}
	return _instance;
}

SerialQueue::SerialQueue()
{
	first = 0;
	sizeofQueue = MAX_SERIAL_QUEUE_SIZE;
	last = sizeofQueue - 1;
	count = 0;
}

int SerialQueue::IsEmpty()
{
	OSLock::LockOwner lock(m_lock);
	if (count <= 0) return (1);
	else return (0);
}


void SerialQueue::Enqueue(const SerialData x)
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
	SerialDataQueue[last] = x;
	count = count + 1;
}

SerialData* SerialQueue::Dequeue()
{
	OSLock::LockOwner lock(m_lock);
	SerialData *result;
	if (count <= 0)
	{
		return NULL;
	}
	else
	{
		result = &SerialDataQueue[first];
		first = (first + 1) % sizeofQueue;
		count = count - 1;
	}

	return(result);
}


int SerialQueue::GetQueueItemNum()
{
	OSLock::LockOwner lock(m_lock);
	return count;
}

