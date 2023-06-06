#include "chat.h"

int main()
{
    Chat chat;  // ������ ������ ������ "���"

    chat.start();

    while (chat.isChatOpen())  // ���� ��� ��������
    {
        chat.showLoginMenu();  // ������ ���� ����� � �������

        while (chat.getCurrentUser())  // ���� ��������� �� �������� ������������ �� nullptr
        {
            chat.showUserMenu();  // ������ ���������������� ����
        }
    }
    return 0;
}
