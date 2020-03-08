// Copyright[2019] <Livadaru Alexandru-Valentin 313CA>

#ifndef PLAYER_H_
#define PLAYER_H_

#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<string>
#include<algorithm>
#include<limits>

class Player {
 private:
    int Score;
    int ID;

 public:
    // Constructor care intizializeaza
    // Scorul si pozitia la ultimul print la 0.
    Player(){
        Score = 0;
    }

    explicit Player(int ID) {
        Score = 0;
        this->ID = ID;
    }

    // Getteri si setteri pt campurile private din clasa.
    int getScore() {
        return this->Score;
    }

    int getID() {
        return this->ID;
    }

    void setID(int ID) {
        this->ID = ID;
    }

    void setScore(int Score) {
        this->Score = Score;
    }
};

// Sortam punctele dupa regula din enutul temei.
bool sortByPoints(Player leftside, Player rightside) {
	if (leftside.getScore() < rightside.getScore()) {
		return 0;
    	}
	if (leftside.getScore() == rightside.getScore()) {
		if (leftside.getID() > rightside.getID()) {
			return 0;
		}
	}
	return 1;
}

class Details {
 private:
    int LastPos;
    int ID;
    int Timer;

 public:
    int getLastPos() {
        return LastPos;
    }

    int getID() {
        return ID;
    }

    int getTimer() {
        return Timer;
    }

    void setTimer(int Timer) {
        this->Timer = Timer;
    }

    void setLastPos(int LastPos) {
        this->LastPos = LastPos;
    }

    void setID(int ID) {
        this->ID = ID;
    }

    friend bool sortByTimer(Details lhs, Details rhs);
};

// Sortam timpii dupa regula din enunt.
bool sortByTimer(Details lhs, Details rhs) {
    if (lhs.Timer > rhs.Timer) {
        return 0;
    }
    if (lhs.Timer == rhs.Timer) {
        if (lhs.getLastPos() > rhs.getLastPos()) {
            return 0;
        }
        if (lhs.getID() > rhs.getID() && rhs.getLastPos() == 0 &&
        rhs.getLastPos() == 0) {
            return 0;
        }
    }
    return 1;
}


#endif  // PLAYER_H_
