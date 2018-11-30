#include <string>
#include "LineManager.h"
#include "Utilities.h"// TEMPORARY!!!

LineManager::LineManager(std::string & fileName, std::vector<Task*>& tasks, std::vector<CustomerOrder>& custToBeFilled)
{
	std::fstream f(fileName);
	if (f.is_open())
	{
		//setup
		CustomerOrder_Count = custToBeFilled.size();
		for (int i = 0; i < custToBeFilled.size(); i++) //int works because vector has an overloaded [] opeartor. Otherwise use auto or iterator
		{
			ToBeFilled.push_back(std::move(custToBeFilled[i]));
		}

		/*****************
		Extraction process
		1-see which two items link to one another from txt file
		2-find the task that has item 1.
		3-Link the pNexttask with the task that has item 2
		4- Loop
		******************/

		//holders
		std::string current_line;
		std::string current_item;
		std::string next_item;

		//init util
		Utilities util;
		bool more = true;
		size_t position = 0;

		for (int i = 0; i < 5; i++) //TODO: Make 5 dynamic
		{
			std::getline(f, current_line, '\n'); //gets next line
			current_item = util.extractToken(current_line, position, more); //takes the line and extracts next token
			if (more == true)
			{
				next_item = util.extractToken(current_line, position, more); //TODO: Might throw error on reading last line
			
			for (auto& curr : AssemblyLine)
			{
				if (curr->getName() == current_item)
					current_item = "";
			}
			for (auto& firstTask : tasks)
			{
				if (current_item == firstTask->getName())
				{
					AssemblyLine.push_back(firstTask);
				}
			}
			for (auto& secondTask : tasks)
			{
				if (next_item == secondTask->getName())
					AssemblyLine.push_back(secondTask);
			}

			AssemblyLine[i]->setNextTask(*AssemblyLine[i+1]);
			}
		}

	}

	for (auto& i : AssemblyLine)
	{
		std::cout << i->getName() <<"\n";
	}

}

bool LineManager::Run(std::ostream & os)
{
	CustomerOrder temp;

	if (!ToBeFilled.empty())
	{
		*AssemblyLine[0] += std::move(ToBeFilled.back());
		ToBeFilled.pop_back();
	}
		//looping over tasks and running the process on each task
		for (auto& currentTask : AssemblyLine)
		{
			currentTask->RunProcess(os);
		}
		for (auto& currentTask : AssemblyLine)
		{
			currentTask->MoveTask();
		}
		if (AssemblyLine.back()->getCompleted(temp))
		{
			Completed.push_back(std::move(temp));
		}

		if (Completed.size() == CustomerOrder_Count)
		{
			for (auto& currCompletedOrder : Completed)
			{
				currCompletedOrder.Display(os);
			}
			return true;
		}

	return false;
}



//bool LineManager::Run(std::ostream & os)
//{
//	if (!ToBeFilled.empty())
//	{
//		// Setting customerOrder to the start of assebmlyLine
//		*AssemblyLine[0] += std::move(ToBeFilled.back()); //TODO: change it to .back()
//
//		//looping over tasks and running the process on each task
//		for (auto currentTask : AssemblyLine)
//		{
//			currentTask->RunProcess(os);
//		}
//
//		//looping 
//		for (auto currentAssembly : AssemblyLine)
//		{
//			currentAssembly->MoveTask();
//		}
//		Completed.push_back(std::move(ToBeFilled.back()));
//	}
//
//	return false;
//}

//LineManager::LineManager(std::string & fileName, std::vector<Task*>& tasks, std::vector<CustomerOrder>& custToBeFilled)
//{
//	std::fstream f(fileName);
//	if (f.is_open())
//	{
//		//setup
//		CustomerOrder_Count = custToBeFilled.size();
//		for (int i = 0; i < custToBeFilled.size(); i++) //int works because vector has an overloaded [] opeartor. Otherwise use auto or iterator
//		{
//			ToBeFilled.push_back(std::move(custToBeFilled[i]));
//			//ToBeFilled[i].Display(std::cout);
//		}
//		AssemblyLine = tasks;
//
//		/*****************
//		Extraction process
//		1-see which two items link to one another from txt file
//		2-find the task that has item 1.
//		3-Link the pNexttask with the task that has item 2
//		4- Loop
//		******************/
//
//		//holders
//		std::string current_line;
//		std::string current_item;
//		std::string next_item;
//
//		//init util
//		Utilities util;
//		bool more = true;
//		size_t position = 0;
//
//		for (int i = 0; i < 5; i++) //TODO: Make 5 dynamic
//		{
//			std::getline(f, current_line, '\n'); //gets next line
//			current_item = util.extractToken(current_line, position, more); //takes the line and extracts next token
//			if (more == true)
//			{
//				next_item = util.extractToken(current_line, position, more); //TODO: Might throw error on reading last line
//			}
//			for (int i = 0; i < AssemblyLine.size(); i++)
//			{
//				if (current_item == AssemblyLine[i]->getName())
//				{
//					
//					//then set pNext task to taks that holds Next_item;
//					for (int j = 0; j < AssemblyLine.size(); j++)
//					{
//						if (next_item == AssemblyLine[j]->getName())
//						{
//							AssemblyLine[i]->setNextTask(*AssemblyLine[j]);
//						}
//					}
//				}
//			}
//		}
//		
//	}
//	else
//	{
//		throw "File did not open properly";
//	}
//
//}
