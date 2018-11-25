#include "LineManager.h"
LineManager::LineManager(std::string & fileName, std::vector<Task*>& tasks, std::vector<CustomerOrder>& custToBeFilled)
{
	std::fstream f(fileName);
	if (f.is_open())
	{

		CustomerOrder_Count = custToBeFilled.size();
		AssemblyLine = tasks;

		int j = 0;
		//setting up pNextTask in  to tasks
		for (auto i : tasks)
		{
			AssemblyLine[j]->setNextTask(*i);
			j++;
		}
		for (auto i : custToBeFilled)
		{
			this->ToBeFilled.push_back(std::move(i)); //TODO: Build error here
		}
	}
	else
	{
		throw "File did not open properly";
	}
}

bool LineManager::Run(std::ostream & os)
{
	if (!ToBeFilled.empty())
	{
		for (auto currentOrder : ToBeFilled)
		{
			for (auto currentTask : AssemblyLine)
			{
				currentTask->getCompleted(currentOrder);
			}
		}
	}

	return false;
}
