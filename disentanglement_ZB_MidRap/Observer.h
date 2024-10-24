/*
Observer pattern abstraction 

Design pattern used: Observer

Dec. 2022
JiaZhao Lin
*/

#ifndef OBSERVER_H
#define OBSERVER_H

template<typename T> 
struct Observer
{
	virtual void    FieldChanged(const T& source, TString field_name)   const = 0;
	virtual         ~Observer	()		= default;
};

template <typename T> 
struct Observable
{
	void Notify	(const T& source, TString name)    const
	{
		for (auto obs : observers)
			obs->FieldChanged(source, name);
	}
	void Subscribe	(Observer< T >* obs) { observers.push_back(obs); }
	void Unsubscribe(Observer< T >* obs)
	{
		auto remove_status = remove(observers.begin(), observers.end(), obs);
		cout<< "The Observer Unsubscribed..." <<endl;
	}
private:
	std::vector< Observer< T > * > observers;
};

#endif