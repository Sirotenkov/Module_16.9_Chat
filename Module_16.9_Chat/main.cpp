#include "chat.h"

int main()
{
    Chat chat;  // Создаю объект класса "Чат"

    chat.start();

    while (chat.isChatOpen())  // Пока чат работает
    {
        chat.showLoginMenu();  // Вывожу меню входа в систему

        while (chat.getCurrentUser())  // Пока указатель на текущего пользователя не nullptr
        {
            chat.showUserMenu();  // Вывожу пользовательское меню
        }
    }
    return 0;
}
