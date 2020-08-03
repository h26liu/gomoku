#pragma once

#include <haohan/Observer.hpp>

#include <set>

class ObservablePublisher {
private:
	std::set<Observer*> obs;
public:
	void Attach(Observer*);
	void Detach(Observer*);
	void Notify();
};