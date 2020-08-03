/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		ObservablePublisher.cpp
*/

#include <haohan/ObservablePublisher.hpp>

void ObservablePublisher::Attach(Observer* o)
{
	obs.insert(o);
}

void ObservablePublisher::Detach(Observer* o)
{
	obs.erase(o);
}

void ObservablePublisher::Notify()
{
	for (auto o : obs)
	{
		o->Update();
	}
}
