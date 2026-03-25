#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <map>
#include <string>

using namespace std;

struct Card {
    string rank;
    string suit;

    int getValue() const {
        if (rank == "A") return 14;
        if (rank == "K") return 13;
        if (rank == "Q") return 12;
        if (rank == "J") return 11;
        if (rank == "10") return 10;
        return atoi(rank.c_str());
    }

    string toString() const {
        return rank + suit;
    }
};

vector<Card> createDeck() {
    vector<Card> deck;
    string ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    string suits[] = { "<3", "^", "+", "/\\" };

    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 4; j++) {
            Card card;
            card.rank = ranks[i];
            card.suit = suits[j];
            deck.push_back(card);
        }
    }
    return deck;
}

void shuffleDeck(vector<Card>& deck) {
    for (int i = 0; i < (int)deck.size(); i++) {
        int j = rand() % deck.size();
        swap(deck[i], deck[j]);
    }
}

void printDeck(const vector<Card>& deck, int count) {
    for (int i = 0; i < count && i < (int)deck.size(); i++) {
        cout << deck[i].toString();
        if (i < count - 1) cout << "\t";
    }
    cout << endl;
}

void printHand(const vector<Card>& hand) {
    for (int i = 0; i < (int)hand.size(); i++) {
        cout << hand[i].toString();
        if (i < (int)hand.size() - 1) cout << "\t";
    }
    cout << endl;
}

string getCombination(const vector<Card>& hand, vector<int>& values) {
    values.clear();
    for (int i = 0; i < (int)hand.size(); i++) {
        values.push_back(hand[i].getValue());
    }
    sort(values.begin(), values.end(), greater<int>());

    map<int, int> freq;
    for (int i = 0; i < (int)values.size(); i++) {
        freq[values[i]]++;
    }

    bool isFlush = true;
    for (int i = 1; i < (int)hand.size(); i++) {
        if (hand[i].suit != hand[0].suit) {
            isFlush = false;
            break;
        }
    }

    vector<int> uniqueValues;
    for (int i = 0; i < (int)values.size(); i++) {
        bool found = false;
        for (int j = 0; j < (int)uniqueValues.size(); j++) {
            if (uniqueValues[j] == values[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            uniqueValues.push_back(values[i]);
        }
    }
    sort(uniqueValues.begin(), uniqueValues.end(), greater<int>());

    bool isStraight = false;
    if ((int)uniqueValues.size() == 5) {
        if (uniqueValues[0] - uniqueValues[4] == 4) {
            isStraight = true;
        }
        if (uniqueValues[0] == 14 && uniqueValues[1] == 5 &&
            uniqueValues[2] == 4 && uniqueValues[3] == 3 && uniqueValues[4] == 2) {
            isStraight = true;
            for (int i = 0; i < 5; i++) {
                if (values[i] == 14) values[i] = 1;
            }
            sort(values.begin(), values.end(), greater<int>());
        }
    }

    if (isFlush && isStraight) return "Straight Flush";
    if ((int)freq.size() == 2) {
        for (map<int, int>::iterator it = freq.begin(); it != freq.end(); it++) {
            if (it->second == 4) return "Four of a Kind";
        }
        return "Full House";
    }
    if (isFlush) return "Flush";
    if (isStraight) return "Straight";
    if ((int)freq.size() == 3) {
        for (map<int, int>::iterator it = freq.begin(); it != freq.end(); it++) {
            if (it->second == 3) return "Three of a Kind";
        }
        return "Two Pair";
    }
    if ((int)freq.size() == 4) return "One Pair";
    return "High Card";
}

int compareHands(const vector<Card>& hand1, const vector<Card>& hand2) {
    vector<int> values1, values2;
    string combo1 = getCombination(hand1, values1);
    string combo2 = getCombination(hand2, values2);

    map<string, int> comboRank;
    comboRank["High Card"] = 1;
    comboRank["One Pair"] = 2;
    comboRank["Two Pair"] = 3;
    comboRank["Three of a Kind"] = 4;
    comboRank["Straight"] = 5;
    comboRank["Flush"] = 6;
    comboRank["Full House"] = 7;
    comboRank["Four of a Kind"] = 8;
    comboRank["Straight Flush"] = 9;

    if (comboRank[combo1] > comboRank[combo2]) return 1;
    if (comboRank[combo1] < comboRank[combo2]) return 2;

    if (combo1 == "One Pair" || combo1 == "Three of a Kind" || combo1 == "Four of a Kind") {
        int pairValue1 = 0, pairValue2 = 0;
        map<int, int> freq1, freq2;

        for (int i = 0; i < (int)values1.size(); i++) freq1[values1[i]]++;
        for (int i = 0; i < (int)values2.size(); i++) freq2[values2[i]]++;

        for (map<int, int>::iterator it = freq1.begin(); it != freq1.end(); it++) {
            if (it->second > 1) {
                pairValue1 = it->first;
                break;
            }
        }

        for (map<int, int>::iterator it = freq2.begin(); it != freq2.end(); it++) {
            if (it->second > 1) {
                pairValue2 = it->first;
                break;
            }
        }

        if (pairValue1 > pairValue2) return 1;
        if (pairValue1 < pairValue2) return 2;

        vector<int> kickers1, kickers2;
        for (int i = 0; i < (int)values1.size(); i++) {
            if (values1[i] != pairValue1) kickers1.push_back(values1[i]);
        }
        for (int i = 0; i < (int)values2.size(); i++) {
            if (values2[i] != pairValue2) kickers2.push_back(values2[i]);
        }

        for (int i = 0; i < (int)kickers1.size() && i < (int)kickers2.size(); i++) {
            if (kickers1[i] > kickers2[i]) return 1;
            if (kickers1[i] < kickers2[i]) return 2;
        }
        return 0;
    }

    if (combo1 == "Two Pair") {
        vector<int> pairs1, pairs2, kicker1, kicker2;
        map<int, int> freq1, freq2;

        for (int i = 0; i < (int)values1.size(); i++) freq1[values1[i]]++;
        for (int i = 0; i < (int)values2.size(); i++) freq2[values2[i]]++;

        for (map<int, int>::iterator it = freq1.begin(); it != freq1.end(); it++) {
            if (it->second == 2) pairs1.push_back(it->first);
            else kicker1.push_back(it->first);
        }

        for (map<int, int>::iterator it = freq2.begin(); it != freq2.end(); it++) {
            if (it->second == 2) pairs2.push_back(it->first);
            else kicker2.push_back(it->first);
        }

        sort(pairs1.begin(), pairs1.end(), greater<int>());
        sort(pairs2.begin(), pairs2.end(), greater<int>());

        for (int i = 0; i < (int)pairs1.size() && i < (int)pairs2.size(); i++) {
            if (pairs1[i] > pairs2[i]) return 1;
            if (pairs1[i] < pairs2[i]) return 2;
        }

        if (kicker1[0] > kicker2[0]) return 1;
        if (kicker1[0] < kicker2[0]) return 2;
        return 0;
    }

    for (int i = 0; i < (int)values1.size() && i < (int)values2.size(); i++) {
        if (values1[i] > values2[i]) return 1;
        if (values1[i] < values2[i]) return 2;
    }

    return 0;
}

int main() {
    srand((unsigned int)time(NULL));

    int player1Score = 0;
    int player2Score = 0;
    int roundNum = 1;

    while (player1Score < 5 && player2Score < 5) {
        cout << "========== ROUND " << roundNum << " ==========" << endl;
        cout << "SCORE: (Player 1) " << player1Score << " : " << player2Score << " (Player 2)" << endl << endl;

        vector<Card> deck = createDeck();
        shuffleDeck(deck);

        cout << "Deck shuffled (showing for interest):" << endl;
        printDeck(deck, 30);
        cout << endl;

        vector<Card> player1Hand;
        vector<Card> player2Hand;

        for (int i = 0; i < 5; i++) {
            player1Hand.push_back(deck[i]);
        }
        for (int i = 5; i < 10; i++) {
            player2Hand.push_back(deck[i]);
        }

        cout << "(PLAYER 1) hand:" << endl;
        printHand(player1Hand);
        cout << endl;

        cout << "(PLAYER 2) hand:" << endl;
        printHand(player2Hand);
        cout << endl;

        vector<int> temp;
        string combo1 = getCombination(player1Hand, temp);
        string combo2 = getCombination(player2Hand, temp);

        cout << ">>> (PLAYER 1) COMBINATION: " << combo1 << endl;
        cout << ">>> (PLAYER 2) COMBINATION: " << combo2 << endl;

        int winner = compareHands(player1Hand, player2Hand);

        if (winner == 1) {
            cout << "*** (PLAYER 1) WON THIS ROUND! ***" << endl;
            player1Score++;
        }
        else if (winner == 2) {
            cout << "*** (PLAYER 2) WON THIS ROUND! ***" << endl;
            player2Score++;
        }
        else {
            cout << "*** TIE! ***" << endl;
        }

        cout << "CURRENT SCORE: (Player 1) " << player1Score << " : " << player2Score << " (Player 2)" << endl;

        if (player1Score >= 5 || player2Score >= 5) {
            cout << endl << "=== GAME OVER ===" << endl;
            if (player1Score > player2Score) {
                cout << "PLAYER 1 WINS THE GAME!" << endl;
            }
            else {
                cout << "PLAYER 2 WINS THE GAME!" << endl;
            }
            break;
        }

        cout << endl << "Press any key for next round..." << endl;
        cin.get();
        cout << endl;
        roundNum++;
    }

    return 0;
}