#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <vector>
using namespace std;

struct Question{
    Question(string stem, char answer):stem(stem),answer(answer){};
    Question(){};
    string stem="Error Question";
    char answer='N';
    bool Judge(char self_answer)
    {
        return self_answer == this->answer;
    }
    char youranswer = 'N';
};

class QuestionBank
{
    public:
        QuestionBank(fstream &fs);
        QuestionBank(string filename);
        int LoadBank(fstream &fs);
        Question RandomQuestion();
        Question FindQuestion(int i);
        int Amount() { return this->amount; };
        bool empty()
        {
            if(amount == 0)
                return true;
            else
                return false;
        }

    private:
        vector<Question> questions;
        int amount;
};


class WrongSet{
public:
    WrongSet(){
        Wrong.clear();
    }
    void addWrong(Question& q){
        Wrong.push_back(q);
    }
    void printWrong(ostream& out) const {
        for(auto elem : Wrong){
            out << elem.stem << endl;
            out << "correct answer: " << elem.answer << endl;
            out << "your answer: " << elem.youranswer << endl;
        }
    }
    void printWrong(const string& filename) const {
        fstream file(filename, ios::app);
        file << endl;
        file << "New Wrong Set: " << endl;
        for(auto elem : Wrong){
            file << elem.stem << endl;
            file << "correct answer: " << elem.answer << endl;
            file << "your answer: " << elem.youranswer << endl;
        }
    }
private:
    vector<Question> Wrong;
};



char FindAnswer(string stem)
{
    stringstream ss;
    ss << stem;
    string temp;
    getline(ss, temp, '(');
    getline(ss, temp, ')');
    char ans;
    stringstream nss;
    nss << temp;
    nss >> ans;
    return ans;
}

string ClearAnswer(string stem)
{
    string newstem,temp;
    stringstream ss;
    ss << stem;
    getline(ss, temp, '(');
    newstem = temp;
    newstem += "(    )";
    getline(ss, temp, ')');
    while(!ss.eof())
    {
    getline(ss, temp);
    newstem += '\n';
    newstem += temp;}
    return newstem;
}

QuestionBank::QuestionBank(fstream &fs)
{
    this->amount=LoadBank(fs);
    srand(time(0));
}

QuestionBank::QuestionBank(string filename)
{
    srand(time(0));
    fstream fs(filename);
    this->amount=LoadBank(fs);
}

int QuestionBank::LoadBank(fstream &fs)
{
    int amount = 0;
    while (!fs.eof())
    {
        string stem;
        getline(fs, stem);
        while(!isdigit(fs.peek()))
        {
            if(fs.eof())
            {
                break;
            }
            string temp;
            getline(fs, temp);
            stem += '\n';
            stem += temp;
        }
        questions.push_back(Question(ClearAnswer(stem), FindAnswer(stem)));
        amount++;
    }
    return amount;
}

Question QuestionBank::RandomQuestion()
{
    auto iter = questions.begin();
    int pos = rand() % this->amount;
    while(pos--)
    {
        iter ++;
    }
    Question ques = *iter;
    amount--;
    questions.erase(iter);
    return ques;
}

Question QuestionBank::FindQuestion(int i)
{
    return this->questions[i];
}

bool DoQuestion(Question temp, WrongSet& wrong)
{
    cout << temp.stem << endl;
    char my_answer;
    cin >> my_answer;
    temp.youranswer = my_answer;
    my_answer = toupper(my_answer);
    if(my_answer=='Q')
    {
        return false;
    }
    if(my_answer=='N')
    {
        return true;
    }
    if(temp.Judge(my_answer))
    {
        cout << "对\n";
        return true;
    }
    else{
        cout << "错\n";
        cout << temp.answer << endl;
        getchar();
        wrong.addWrong(temp);
        return true;
    }
}
