#include "Task.h"

Task::Task(std::string & record) : Item(record)
{
	pNextTask = nullptr;
	//Orders are dynamic so no need to set them
}

void Task::RunProcess(std::ostream & os)
{
	//checking if there are customer orders in queue
	if (!Orders.empty())
	{
		//checking order status for that item
		if (Orders.back().getItemFillState(this->getName()) == false)
		{
			//filling item
			Orders.back().fillItem(*this, os);
		}
	}
}

//bool Task::MoveTask()
//{
//	if (Orders.empty())
//		return false;
//
//	//moving last order to pNextTask which should hold the next task
//	if (Orders.back().getOrderFillState() == true)
//		*pNextTask += std::move(Orders.back());
//}

bool Task::MoveTask() {
	if (Orders.empty())
		return false;

	if (Orders.back().getItemFillState(this->getName()) == true)
	{
		if (pNextTask)
		{
			*pNextTask += std::move(Orders.back());
			Orders.pop_back();
			return true;
		}
	}

}

void Task::setNextTask(Task & NextTaskObj)
{
	pNextTask = &NextTaskObj; //TODO: Check if this is how you set a pointer to a reference
}

bool Task::getCompleted(CustomerOrder & src)
{
	if (this->Orders.empty())
		return false;
	src = std::move(Orders.back());
	Orders.pop_back();
	return true;
}

void Task::Validate(std::ostream & os)
{
	os <<"Current task Item name" << this->getName() << std::endl;
	if (pNextTask != nullptr) //if next task is available
		os <<"Next " << pNextTask->getName() << std::endl;
}

Task & Task::operator+=(CustomerOrder && srcOrder)
{
	this->Orders.push_front(std::move(srcOrder)); //TODO: Build error here
	return *this;
}