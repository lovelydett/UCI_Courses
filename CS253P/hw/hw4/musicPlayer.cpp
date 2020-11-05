/*************************************************
Author: Yuting Xie
Date:2020-11-2
Description: for 253P HW4: musicPlayer
**************************************************/
#include<iostream>
#include<cstdio>
#include<vector>
#include<string>

#include<cstdlib> // for rand()

using std::vector;
using std::string;

inline void myFlush() {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

struct Song {
    string name;
    string author;
    Song(string _name, string _author) {
        name = _name;
        author = _author;
    }
    Song(string complexStr) {
        auto i = complexStr.find('['), j = complexStr.find(']');
        name.assign(complexStr.begin(), complexStr.begin() + i);
        author.assign(complexStr.begin() + i + 1, complexStr.begin() + j);
    }
    Song& operator=(Song& song) {
        name = song.name;
        author = song.author;
        return *this;
    }
    string toString() {
        return name + "[" + author + "]";
    }
    bool operator==(Song& song) {
        return name == song.name && author == song.author;
    }
};

typedef struct MyLinkedListNode {
    Song song;
    MyLinkedListNode* next;
    MyLinkedListNode(Song& song) :song("default", "default") {
        this->song = song;
        next = nullptr;
    };
}Node;

class SimplePlayList
{
public:
    SimplePlayList() :listHead(nullptr), listCur(nullptr) {}
    ~SimplePlayList();

    void readCmd();

private:

    void evaluateCmd(string cmd);
    void evaluateCmd(string cmd, Song& song);
    void evaluateCmd(string cmd, Song& song1, Song& song2);

    void pushFront(Song& song);  //push
    void pushBack(Song& song);   //queue
    void displayCurrent();
    void display(Node* prev, Node* cur, Node* next);
    void deleteCurrent();
    void prev();
    void next();
    void restart();
    void find(Song& song);
    void changeTo(Song& song);
    void addBefore(Song& src, Song& dst);
    void addAfter(Song& src, Song& dst);
    void random();
    void print();

    Node* listHead;
    Node* listCur;
    Node* listTail;

    Node* getPrev(Node* node);
    Node* getTail();

};

SimplePlayList::~SimplePlayList()
{
    if (!listHead)
        return;

    Node* lptr = listHead, * rptr = listHead->next;
    while (rptr) {
        delete(lptr);
        lptr = rptr;
        rptr = rptr->next;
    }
    delete(lptr);

}

void SimplePlayList::readCmd()
{
    string cmd, attr1, attr2;

    auto getTwo = [&]() {
        std::getline(std::cin, attr1, ']');
        std::getline(std::cin, attr2, ']');
        attr1.append("]");
        attr2.append("]");
    };
    auto getOne = [&]() {
        std::getline(std::cin, attr1, ']');
        attr1.append("]");
    };

    while (true) {
        std::cin >> cmd;

        if (cmd == string("push")) {
            getOne();
            Song song(attr1);
            pushFront(song);

        }
        else if (cmd == string("queue")) {
            getOne();
            Song song(attr1);
            pushBack(song);

        }
        else if (cmd == string("prev")) {
            prev();

        }
        else if (cmd == string("next")) {
            next();

        }
        else if (cmd == string("current")) {
            displayCurrent();

        }
        else if (cmd == string("addBefore")) {
            getTwo();
            Song song1(attr1), song2(attr2);
            addBefore(song1, song2);

        }
        else if (cmd == string("addAfter")) {
            getTwo();
            Song song1(attr1), song2(attr2);
            addAfter(song1, song2);

        }
        else if (cmd == string("find")) {
            getOne();
            Song song(attr1);
            find(song);
        }
        else if (cmd == string("print")) {
            print();
        }
        else if (cmd == "changeTo") {
            getOne();
            Song song(attr1);
            changeTo(song);
        }
        else if(cmd == "delete") {
            deleteCurrent();
        }
        else if (cmd == "exit") {
            printf("\texiting...\n");
            break;
        }

        //flush the stdin
        //myFlush();

    }
}


void SimplePlayList::pushFront(Song& song)
{
    if (!listHead) {
        listTail = listHead = listCur = new Node(song);
        return;
    }
    Node* tmp = new Node(song);

    tmp->next = listHead;
    listHead = tmp;
}

void SimplePlayList::pushBack(Song& song)
{
    if (!listHead) {
        listTail = listHead = listCur = new Node(song);
        return;
    }

    //append
    listTail->next = new Node(song);
    listTail = listTail->next;
}

void SimplePlayList::displayCurrent()
{
    if (!listHead || !listCur)
        return;

    Node* prev = getPrev(listCur);
    Node* next = listCur->next ? listCur->next : listHead;
    display(prev, listCur, next);
}

void SimplePlayList::display(Node* prev, Node* cur, Node* next)
{
    printf("\tprevious: %s\n", prev->song.toString().c_str());
    printf("\tcurrent:  %s\n", cur->song.toString().c_str());
    printf("\tnext:     %s\n", next->song.toString().c_str());
}

void SimplePlayList::deleteCurrent()
{
    if (!listHead)
        return;

    if (listHead == listCur) {
        listCur = listHead->next;
        delete(listHead);
        listHead = listCur;
        return;
    }

    Node* prev = getPrev(listCur);
    prev->next = listCur->next;
    delete(listCur);
    listCur = prev->next == nullptr ? listHead : prev->next;
}

void SimplePlayList::prev()
{
    if (!listHead) {
        printf("\tcurrent no song in list!\n");
        return;
    }

    listCur = getPrev(listCur);
}

void SimplePlayList::next()
{
    if (!listHead) {
        printf("\tcurrent no song in list!\n");
        return;
    }

    if (listCur->next == nullptr)
        listCur = listHead;
    else
        listCur = listCur->next;


}

void SimplePlayList::restart()
{
    if (!listHead) {
        printf("\tcurrent no song in list!\n");
        return;
    }

    listCur = listHead;
}

//in O(n) time finds the queried song, and in O(1) time from when the song is found, is able to display it, its previous song, and its next song.
void SimplePlayList::find(Song& song)
{
    if (!listHead) {
        printf("\tcannot find song\n");
        return;
    }

    //first check head
    if (listHead->song == song) {
        display(listTail, listHead, listHead->next ? listHead->next : listHead);
        return;
    }

    Node* prev = listHead, * cur = listHead->next;
    while (cur) {
        if (cur->song == song) {
            display(prev, cur, cur->next ? cur->next : listHead);
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    printf("\tcannot find song\n");

}

//changes current song to the song entered by the user in O(n) time
void SimplePlayList::changeTo(Song& song)
{
    if (!listHead) {
        printf("\tno song in list\n");
        return;
    }

    for (Node* cur = listHead; cur; cur = cur->next) {
        if (cur->song == song) {
            listCur = cur;
            return;
        }
    }

    printf("\tno this song in list\n");
}

void SimplePlayList::addBefore(Song& src, Song& dst)
{
    if (!listHead) {
        printf("\tno song in list, enable to addBefore\n");
        return;
    }

    //if add at head
    if (listHead->song == dst) {
        Node* tmp = new Node(src);
        tmp->next = listHead;
        listHead = tmp;
        return;
    }

    //find place to add
    Node* prev = listHead, * cur = listHead->next;
    while (cur) {
        if (dst == cur->song) {
            Node* tmp = new Node(src);
            tmp->next = cur;
            prev->next = tmp;
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    printf("\tno this song in list, enable to addBefore\n");
}

void SimplePlayList::addAfter(Song& src, Song& dst)
{
    if (!listHead) {
        printf("\tno song in list, enable to addAfter\n");
        return;
    }

    for (Node* cur = listHead; cur; cur = cur->next) {

        if (cur->song == dst) {
            Node* tmp = new Node(src);
            tmp->next = cur->next;
            cur->next = tmp;

            //if new tail
            if (!tmp->next)
                listTail = tmp;

            return;
        }

    }

    printf("\tno this song in list, enable to addAfter\n");
}

void SimplePlayList::random()
{
    if (!listHead)
        return;

    unsigned randInt = (unsigned)rand();
    Node* tmp = listHead;
    int numNode = 0;

    for (int i = 0; i < randInt; i++) {

        numNode++;

        if (!(tmp->next)) {
            tmp = listHead;
            randInt = randInt % numNode;
            i = -1;
        }
        else
        {
            tmp = tmp->next;
        }

    }

    listCur = tmp;

}

void SimplePlayList::print()
{
    if (!listHead) {
        printf("\tno song in list, nothing to print\n");
        return;
    }

    Node* cur = listHead;
    int num = 1;
    while (cur) {
        printf("\t%d. %s\n", num, cur->song.toString().c_str());
        cur = cur->next;
        num++;
    }
}


//private below

Node* SimplePlayList::getPrev(Node* node)
{
    if (!listHead || !node)
        return nullptr;

    if (node == listHead)
        return listTail;

    Node* tmp = listHead;
    while (tmp->next && tmp->next != node)
        tmp = tmp -> next;

    return tmp;

}

Node* SimplePlayList::getTail()
{
    if (!listHead)
        return nullptr;

    //find tail
    Node* tmp = listHead;
    while (tmp->next)
        tmp = tmp->next;

    return tmp;
}


int main() {
    SimplePlayList player;
    player.readCmd();
    return 0;
}


//test input:
/*

push Song Title A [Author A]
push Song Title B [Author B]
queue Song Title C [Author C]
queue Song Title D [Author D]
print
next
current
prev
addBefore AddBefore Song [Before] Song Title B [Author B]
print
addAfter AddAfter Song [After] Song Title B [Author B]
print
current
delete
print
changeTo Song Title A [Author A]
current
print
find Song Title C [Author C]
find Not Exist [Nobody]
print

*/