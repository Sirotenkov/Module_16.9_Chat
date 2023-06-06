#include <iostream>
#include "chat.h"

void Chat::start()
{
	_isChatOpen = true;
}

User const* Chat::getUserByLogin(const std::string& login) const  // Описание метода, возвращающего указатель на текущего пользователя по его уникальному логину
{                                                                 //
	for (auto& user : _users)                                     //
	{                                                             //
		if (login == user.getUserLogin())                         //
		{                                                         //
			return &user;                                         // Возвращаю указатель на текущего пользователя по уникальному логину
		}                                                         //
	}                                                             //
	return nullptr;                                               // Если пользователя с таким логином не нашлось в контейнере для хранения данных о пользователях, то возвращаю указателю значение nullptr
}

void Chat::login()                     // Описание функции входа в систему
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

		if (_currentUser == nullptr || !isEqualPassword(password, _currentUser->getPassword().data()))   // Если указатель на текущего пользователя - nullptr или введён неверный пароль
		{
			std::cout << "Login failed!..." << std::endl;                               // Вывожу ошибку
			std::cout << "0 - exit this menu, or (any kay) for repeat: " << std::endl;  // Предлагаю выйти из меню или попробовать снова
			std::cin >> operation;

			if (operation == '0') break;
		}
	} while (!_currentUser);
}

void Chat::signUp()    // Описание функции регистрации пользователя в системе
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

	_users.push_back({ login, password });  // Добавляю в конец контейнера хранения данных о пользователях
	_currentUser = &_users.back();          // Указатель на текущего пользователя указывает на этого добавленного пользователя
}

void Chat::showChat()  // Описание функции отображения меню чата
{
	std::string from, to;

	std::cout << "----- Chat -----" << std::endl;

	for (auto& mess : _messages)
	{
		if (_currentUser->getUserLogin() == mess.getUserFrom() || _currentUser->getUserLogin() == mess.getUserTo() || mess.getUserTo() == "all")  // По указателю на объект "пользователь" беру его уникальный логин и сортирую сообщения от него, ему и всем
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

void Chat::showLoginMenu()     // Описание функции вывода меню чата на экран
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
	} while (!_currentUser && _isChatOpen);  // Цикл работает пока пользователь корректен и он существует и чат работает
}

void Chat::showUserMenu()      // Описание функции, отображающей пользовательское меню
{
	char operation;

	std::cout << "Hi, " << _currentUser->getUserLogin() << std::endl;  // Вывод приветствия для вошедшего в систему пользователя по его логину

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

void Chat::showAllUsers()     // Описание функции отображения всех пользователей системы
{
	std::cout << "----- All Users -----" << std::endl;
	for (auto& user : _users)
	{
		std::cout << user.getUserLogin() << std::endl;

		if (_currentUser->getUserLogin() == user.getUserLogin())  // Проверка для определения пользователя, под логином которого в данный момент осуществлён вход в систему
		{
			std::cout << " (me) " << std::endl;
		}
	}
	std::cout << "-----------------------" << std::endl;
}

void Chat::addMessage()        // Описание функции добавления сообщений
{
	std::string to, text;

	std::cout << "To (name or \"all\")" << std::endl;  // Прошу ввести получателя
	std::cin >> to;
	std::cout << "Text message: ";  // Прошу ввести текст сообщения
	std::cin.ignore();
	getline(std::cin, text);  // Считываю текст сообщения с консоли

	if (!(to == "all" || getUserByLogin(to)))    // Если не удалось найти получателя по логину или в качестве адресата не указаны "все"
	{
		std::cout << "Error send message: can't find " << to << std::endl;  // Вывожу сообщение об ошибке
		return;
	}

	if (to == "all")    // Если получателем указаны "все"
		_messages.push_back(Message{ _currentUser->getUserLogin(), "all", text });  // То добавляю в конец контейнера для хранения сообщений сообщение с соответствующими атрибутами
	else
		_messages.push_back(Message{ _currentUser->getUserLogin(), getUserByLogin(to)->getUserLogin(), text });  // Если в качестве получателя указан конкретный пользователь, то добавляю в контейнер хрпнения сообщений сообщение с данными атрибутами
}
