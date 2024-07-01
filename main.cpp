#include "QuestionBank.h"

#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

int main()
{
    system("chcp 65001");
    QuestionBank QB("Bank.txt");
    WrongSet wrong;
    cout << "在答题时使用q退出,使用n跳过题目。"<<endl;
    cout << "请选择顺序(S)或乱序(L)" << endl;
    char command;
    cin >> command;
    command = toupper(command);
    if(command=='L')
    {
        while(!QB.empty())
        {
            Question temp = QB.RandomQuestion();
            if (!DoQuestion(temp, wrong))
            {
                break;
            }
            cout << "还剩 " << QB.Amount() << " L题\n";
        }
    }
    if(command=='S')
    {
        int begin;
        cout << "请输入题号" << endl;
        cin >> begin;
        for (int i = begin; i < QB.Amount();i++)
        {
            cout << i << ':';
            auto temp=QB.FindQuestion(i);
            if (!DoQuestion(temp, wrong))
            {
                break;
            }
        }
    }
    cout << "Wrong Set Print Form: (F for fileprint, A for cmdprint)" << endl;
    cin >> command;
    command = toupper(command);
    if(command == 'F'){
        wrong.printWrong("Wrong.txt");
    }
    else if(command == 'A'){
        wrong.printWrong(cout);
    }
    
    return 0;
}