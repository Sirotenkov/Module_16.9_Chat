#include <iostream>
#include "chat.h"

void Chat::start()
{
	_isChatOpen = true;
}

User const* Chat::getUserByLogin(const std::string& login) const  // �������� ������, ������������� ��������� �� �������� ������������ �� ��� ����������� ������
{                                                                 //
	for (auto& user : _users)                                     //
	{                                                             //
		if (login == user.getUserLogin())                         //
		{                                                         //
			return &user;                                         // ��������� ��������� �� �������� ������������ �� ����������� ������
		}                                                         //
	}                                                             //
	return nullptr;                                               // ���� ������������ � ����� ������� �� ������� � ���������� ��� �������� ������ � �������������, �� ��������� ��������� �������� nullptr
}

void Chat::login()                     // �������� ������� ����� � �������
{
	std::string login, password;
	char operation;

	do
	{
		std::cout << "Login: ";
		std::cin >> login;
		std::cout << "Password: ";
		std::cin >> password;

		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr || !isEqualPassword(password, _currentUser->getPassword().data()))   // ���� ��������� �� �������� ������������ - nullptr ��� ����� �������� ������
		{
			std::cout << "Login failed!..." << std::endl;                               // ������ ������
			std::cout << "0 - exit this menu, or (any kay) for repeat: " << std::endl;  // ��������� ����� �� ���� ��� ����������� �����
			std::cin >> operation;

			if (operation == '0') break;
		}
	} while (!_currentUser);
}

void Chat::signUp()    // �������� ������� ����������� ������������ � �������
{
	std::string login, password;

	std::cout << "Login: ";
	std::cin >> login;
	std::cout << "Password: ";
	std::cin >> password;

	if (getUserByLogin(login) || login == "all")
	{
		throw UserLoginExceptions();
	}

	_users.push_back({ login, password });  // �������� � ����� ���������� �������� ������ � �������������
	_currentUser = &_users.back();          // ��������� �� �������� ������������ ��������� �� ����� ������������ ������������
}

void Chat::showChat()  // �������� ������� ����������� ���� ����
{
	std::string from, to;

	std::cout << "----- Chat -----" << std::endl;

	for (auto& mess : _messages)
	{
		if (_currentUser->getUserLogin() == mess.getUserFrom() || _currentUser->getUserLogin() == mess.getUserTo() || mess.getUserTo() == "all")  // �� ��������� �� ������ "������������" ���� ��� ���������� ����� � �������� ��������� �� ����, ��� � ����
		{
			from = (_currentUser->getUserLogin() == mess.getUserFrom()) ? "me " : getUserByLogin(mess.getUserFrom())->getUserLogin();

			if (mess.getUserTo() == "all")
			{
				to = "all";
			}
			else
			{
				to = (_currentUser->getUserLogin() == mess.getUserTo()) ? "me " : getUserByLogin(mess.getUserTo())->getUserLogin();
			}

			std::cout << "Message from " << from << " to " << to << std::endl;
			std::cout << "Text message: " << mess.getUserMessage() << std::endl;
		}
	}

	std::cout << "-------------------------" << std::endl;
}

void Chat::showLoginMenu()     // �������� ������� ������ ���� ���� �� �����
{
	_currentUser = nullptr;

	char operation;

	do
	{
		std::cout << "1 - Login" << std::endl;
		std::cout << "2 - SignUp" << std::endl;
		std::cout << "0 - Exit" << std::endl;

		std::cin >> operation;

		switch (operation)
		{
		case '1':
			login();
			break;
		case '2':
			try
			{
				signUp();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
			break;
		case '0':
			_isChatOpen = false;
			break;
		default:
			std::cout << "Input 1 or 2 " << std::endl;
			break;
		}
	} while (!_currentUser && _isChatOpen);  // ���� �������� ���� ������������ ��������� � �� ���������� � ��� ��������
}

void Chat::showUserMenu()      // �������� �������, ������������ ���������������� ����
{
	char operation;

	std::cout << "Hi, " << _currentUser->getUserLogin() << std::endl;  // ����� ����������� ��� ��������� � ������� ������������ �� ��� ������

	while (_currentUser)
	{
		std::cout << "User menu: 1 - Show chat, 2 - Add message, 3 - Show All users, 0 - Exit" << std::endl;

		std::cin >> operation;

		switch (operation)
		{
		case '1':
			showChat();
			break;
		case '2':
			addMessage();
			break;
		case '3':
			showAllUsers();
			break;
		case '0':
			_currentUser = nullptr;
			break;
		default:
			std::cout << "Unknown choice! " << std::endl;
		}
	}
}

void Chat::showAllUsers()     // �������� ������� ����������� ���� ������������� �������
{
	std::cout << "----- All Users -----" << std::endl;
	for (auto& user : _users)
	{
		std::cout << user.getUserLogin() << std::endl;

		if (_currentUser->getUserLogin() == user.getUserLogin())  // �������� ��� ����������� ������������, ��� ������� �������� � ������ ������ ���������� ���� � �������
		{
			std::cout << " (me) " << std::endl;
		}
	}
	std::cout << "-----------------------" << std::endl;
}

void Chat::addMessage()        // �������� ������� ���������� ���������
{
	std::string to, text;

	std::cout << "To (name or \"all\")" << std::endl;  // ����� ������ ����������
	std::cin >> to;
	std::cout << "Text message: ";  // ����� ������ ����� ���������
	std::cin.ignore();
	getline(std::cin, text);  // �������� ����� ��������� � �������

	if (!(to == "all" || getUserByLogin(to)))    // ���� �� ������� ����� ���������� �� ������ ��� � �������� �������� �� ������� "���"
	{
		std::cout << "Error send message: can't find " << to << std::endl;  // ������ ��������� �� ������
		return;
	}

	if (to == "all")    // ���� ����������� ������� "���"
		_messages.push_back(Message{ _currentUser->getUserLogin(), "all", text });  // �� �������� � ����� ���������� ��� �������� ��������� ��������� � ���������������� ����������
	else
		_messages.push_back(Message{ _currentUser->getUserLogin(), getUserByLogin(to)->getUserLogin(), text });  // ���� � �������� ���������� ������ ���������� ������������, �� �������� � ��������� �������� ��������� ��������� � ������� ����������
}
