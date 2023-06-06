#pragma once
#include "user.h"

class Message
{
private:
	const std::string _from;      // От кого
	const std::string _to;        // Кому
	const std::string _message;   // Текст сообщения
public:
	Message(const std::string& from, const std::string& to, const std::string& message) :
		_from(from), _to(to), _message(message) {}

	const std::string& getUserFrom() const
	{
		return _from;
	}

	const std::string& getUserTo() const
	{
		return _to;
	}

	const std::string& getUserMessage() const
	{
		return _message;
	}
};
