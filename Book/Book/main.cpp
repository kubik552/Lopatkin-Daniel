#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Publication {
protected:
    string name;
    string house;
    int releaseYear;
    double cost;

public:
    Publication(const char* n = "", const char* h = "", int y = 0, double c = 0.0)
        : name(n), house(h), releaseYear(y), cost(c) {
    }

    const string& getName() const { return name; }
    const string& getHouse() const { return house; }
    int getYear() const { return releaseYear; }
    double getCost() const { return cost; }

    void setName(const char* n) { name = n; }
    void setHouse(const char* h) { house = h; }
    void setYear(int y) { releaseYear = y; }
    void setCost(double c) { cost = c; }

    virtual void show() const
    {
        cout << "Name: " << name << "\nPublisher: " << house << "\nYear: " << releaseYear << "\nPrice: " << cost << " rub.\n";
    }

    virtual const char* category() const { return "Publication"; }
};

class Novel : public Publication {
    string writer;
    int pageCount;

public:
    Novel(const char* n = "", const char* h = "", int y = 0, double c = 0.0,
        const char* w = "", int p = 0)
        : Publication(n, h, y, c), writer(w), pageCount(p) {
    }

    const string& getWriter() const { return writer; }
    int getPages() const { return pageCount; }

    void setWriter(const char* w) { writer = w; }
    void setPages(int p) { pageCount = p; }

    void show() const override {
        cout << "\nNOVEL\n";
        cout << "Title: " << getName() << "\nAuthor: " << writer << "\nPublisher: " << getHouse() << "\nYear: " << getYear() << "\nPages: " << pageCount << "\nPrice: " << getCost() << " rub.\n";
        if (pageCount > 0)
        {
            cout << "Value: " << getCost() / pageCount << " rub/page\n";
        }
    }

    const char* category() const override { return "Novel"; }

    bool isLong() const { return pageCount > 500; }
};

class Magazine : public Publication {
    string frequency;

public:
    Magazine(const char* n = "", const char* h = "", int y = 0, double c = 0.0,
        const char* freq = "")
        : Publication(n, h, y, c), frequency(freq) {
    }

    const string& getFrequency() const { return frequency; }
    void setFrequency(const char* f) { frequency = f; }

    void show() const override {
        cout << "\nMAGAZINE\n";
        cout << "Title: " << getName() << "\nPublisher: " << getHouse() << "\nYear: " << getYear() << "\nFrequency: " << frequency << "\nPrice: " << getCost() << " rub.\n";
    }

    const char* category() const override { return "Magazine"; }

    void setDiscount(double percent) {
        setCost(getCost() * (100.0 - percent) / 100.0);
    }
};

int main()
{
    cout << "PUBLICATION CATALOG\n";

    Novel bookA("The Master and Margarita", "Eksmo", 2019, 450,
        "Mikhail Bulgakov", 416);

    Novel bookB("War and Peace", "AST", 2020, 890,
        "Leo Tolstoy", 1300);

    Magazine magX("National Geographic", "NG Media", 2024, 350,
        "monthly");

    Magazine magY("Vogue", "Conde Nast", 2024, 520,
        "monthly");

    bookA.show();
    bookB.show();
    magX.show();
    magY.show();

    cout << "\nEXTRA FEATURES\n";

    cout << "\nLong novels (over 500 pages):\n";
    if (bookA.isLong()) cout << "  • " << bookA.getName() << "\n";
    if (bookB.isLong()) cout << "  • " << bookB.getName() << "\n";

    cout << "\nApply discount to magazines:\n";
    cout << "  " << magX.getName() << ": " << magX.getCost() << " rub";
    magX.setDiscount(15);
    cout << " → " << magX.getCost() << " rub (15% off)\n";

    cout << "\nChange frequency:\n";
    magY.setFrequency("weekly");
    cout << "  " << magY.getName() << " now " << magY.getFrequency() << "\n";

    return 0;
}
