#include <string>
#include "LineManager.h"
#include "Utilities.h"// TEMPORARY!!!

//My own compare function because == from string lib was not acting as expected even after hours of debugging
static bool myStringCompare(std::string currentItem, std::string assemblyLine) { 
	int j = 0;
	if (currentItem.size() != assemblyLine.size())
		return false;
	for (int i = 0; i < currentItem.size(); i++)
	{
		if (currentItem[i] != assemblyLine[j])
			return false;
	}
	return true;
}


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
			//ToBeFilled[i].Display(std::cout);
		}
		AssemblyLine = tasks;

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
		std::string assemblyNameI;
		std::string assemblyNameJ;

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
			}
			for (int i = 0; i < AssemblyLine.size(); i++)
			{
				if (current_item == AssemblyLine[i]->getName())
				{
					//then set pNext task to taks that holds Next_item;
					for (int j = 0; j < AssemblyLine.size(); j++)
					{
						assemblyNameJ = AssemblyLine[j]->getName();
						if (next_item == assemblyNameJ)
						{
							AssemblyLine[i]->setNextTask(*AssemblyLine[j]);
							std::cout <<"CurrnetItem: " << AssemblyLine[i]->getName() <<'\n'<<"AssemblyLineJ>>> " << assemblyNameJ << std::endl;
						}
					}
				}
			}
		}
		
	}
	else
	{
		throw "File did not open properly";
	}

}


bool LineManager::Run(std::ostream & os)
{
	static int temp = 0; //TEMP
	if(!ToBeFilled.empty())
	{
		// Setting customerOrder to the start of assebmlyLine
		*AssemblyLine[0] += std::move(ToBeFilled.back()); //TODO: change it to .back()

		//looping over tasks and running the process on each task
		for (auto currentAssembly : AssemblyLine)
		{
			currentAssembly->RunProcess(os);
		}

		//looping 
		for (auto currentAssembly : AssemblyLine)
		{
			currentAssembly->MoveTask();
		}
		temp++;
			if (temp == 5)
				return true;
		//if (ToBeFilled.back().getOrderFillState() == true)
		//{
		//	this->Completed.push_back(std::move(ToBeFilled.back()));
		//}
	}
	return false;
}
//bool LineManager::Run(std::ostream & os)
//{
//	if (!ToBeFilled.empty())
//	{
//		for (auto& currentOrder : ToBeFilled)
//		{
//			for (auto& currentTask : AssemblyLine)
//			{
//				if(//items are same)
//				currentTask->getCompleted(currentOrder);
//			}
//		}
//	}
//	return false;
//}
