#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <cmath>
#define M_PI 3.14159265358979323846264338327950288

class Point {
public:
    double x, y;

    Point(double x, double y) : x(x), y(y) {}
    Point() : x(0), y(0) {}
};

class Line {
public:
    double a, b, c;

    Line(double a, double b, double c) : a(a), b(b), c(c) {}
    Line() : a(0), b(0), c(0) {}

    double slope() const {
        return -a / b;
    }
};

class Circle {
public:
    Point center;
    double radius;

    Circle(const Point& center, double radius) : center(center), radius(radius) {}
    Circle() : radius(0) {}
};

class UserData {
public:
    Point userPoint;
    Line userLine;
    Circle userCircle;
    Point reflectedPoint;
    Point invertedPoint;
    bool intersection;
    Line userLine2;
    double angle;

    void intersectLineCircle() {
        double a = userLine.a;
        double b = userLine.b;
        double c = userLine.c;
        double x0 = userCircle.center.x;
        double y0 = userCircle.center.y;
        double r = userCircle.radius;

        double d = std::fabs(a * x0 + b * y0 + c) / std::sqrt(a * a + b * b);

        if (d > r) {
            intersection = false;
        } else {
            double x1 = x0 - (a * (a * x0 + b * y0 + c)) / (a * a + b * b);
            double y1 = y0 - (b * (a * x0 + b * y0 + c)) / (a * a + b * b);

            double x2 = x0 + (a * (a * x0 + b * y0 + c)) / (a * a + b * b);
            double y2 = y0 + (b * (a * x0 + b * y0 + c)) / (a * a + b * b);

            intersection = true;
            reflectedPoint = Point(x1, y1);
            invertedPoint = Point(x2, y2);
        }
    }

    Point reflectPoint() const {
        double a = userLine.a;
        double b = userLine.b;
        double c = userLine.c;

        double x = userPoint.x - 2 * a * (a * userPoint.x + b * userPoint.y + c) / (a * a + b * b);
        double y = userPoint.y - 2 * b * (a * userPoint.x + b * userPoint.y + c) / (a * a + b * b);

        return Point(x, y);
    }

    Point invertPoint() const {
        double x0 = userCircle.center.x;
        double y0 = userCircle.center.y;
        double r = userCircle.radius;

        if (userPoint.x == x0 && userPoint.y == y0) {
            return userPoint;
        }

        double d = std::sqrt((userPoint.x - x0) * (userPoint.x - x0) + (userPoint.y - y0) * (userPoint.y - y0));
        double x = x0 + (r * r * (userPoint.x - x0)) / (d * d);
        double y = y0 + (r * r * (userPoint.y - y0)) / (d * d);

        return Point(x, y);
    }

    void calculateAngle() {
        double slope1 = userLine.slope();
        double slope2 = userLine2.slope();

        angle = std::atan(std::fabs((slope2 - slope1) / (1 + slope1 * slope2)));
    }

    void printUserData() const {
        std::cout << "1. Coordinates: (" << userPoint.x << ", " << userPoint.y << ")" << std::endl;
        std::cout << "2. Line equation: " << userLine.a << "x + " << userLine.b << "y + " << userLine.c << " = 0" << std::endl;
        std::cout << "3. Circle equation: (x - " << userCircle.center.x << ")^2 + (y - " << userCircle.center.y << ")^2 = "
                  << userCircle.radius * userCircle.radius << std::endl;

        Point reflected = reflectPoint();
        std::cout << "4. Reflect point (" << userPoint.x << ", " << userPoint.y << ") about the line: (" << reflected.x << ", " << reflected.y << ")" << std::endl;

        Point inverted = invertPoint();
        std::cout << "5. Invert point (" << userPoint.x << ", " << userPoint.y << ") about the circle: (" << inverted.x << ", " << inverted.y << ")" << std::endl;

        if (intersection) {
            std::cout << "6. Intersection points: (" << reflected.x << ", " << reflected.y << ") and (" << inverted.x << ", " << inverted.y << ")" << std::endl;
        } else {
            std::cout << "6. No intersection between line and circle." << std::endl;
        }

        std::cout << "7. Second line equation: " << userLine2.a << "x + " << userLine2.b << "y + " << userLine2.c << " = 0" << std::endl;
        std::cout << "8. Angle between lines: " << angle << " radians" << std::endl;
    }
};

int customPartition(std::vector<UserData>& userDataList, int low, int high) {
    double pivot = userDataList[high].angle;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (userDataList[j].angle >= pivot) {
            i++;
            std::swap(userDataList[i], userDataList[j]);
        }
    }
    std::swap(userDataList[i + 1], userDataList[high]);
    return i + 1;
}

void quickSortUserData(std::vector<UserData>& userDataList, int low, int high) {
    if (low < high) {
        int partitionIndex = customPartition(userDataList, low, high);
        quickSortUserData(userDataList, low, partitionIndex - 1);
        quickSortUserData(userDataList, partitionIndex + 1, high);
    }
}

void addUserData(std::vector<UserData>& userDataList) {
    UserData newUser;

    std::cout << "Enter new UserData:" << std::endl;
    std::cout << "=====================" << std::endl;

    std::cout << "Enter coordinates (x y): ";
    std::cin >> newUser.userPoint.x >> newUser.userPoint.y;

    std::cout << "Enter coefficients of the line (ax + by + c = 0): ";
    std::cin >> newUser.userLine.a >> newUser.userLine.b >> newUser.userLine.c;

    std::cout << "Enter coordinates of the circle center and its radius (x y r): ";
    std::cin >> newUser.userCircle.center.x >> newUser.userCircle.center.y >> newUser.userCircle.radius;

    std::cout << "Enter coefficients of the second line (ax + by + c = 0): ";
    std::cin >> newUser.userLine2.a >> newUser.userLine2.b >> newUser.userLine2.c;

    newUser.intersectLineCircle();
    newUser.calculateAngle();

    userDataList.push_back(newUser);

    std::cout << "New UserData added successfully!" << std::endl << std::endl;
}

void showAllUserData(const std::vector<UserData>& userDataList) {
    if (userDataList.empty()) {
        std::cout << "The list is empty!" << std::endl << std::endl;
    } else {
        std::cout << "All UserData entries:" << std::endl;
        for (size_t i = 0; i < userDataList.size(); ++i) {
            std::cout << "UserData " << (i + 1) << ":" << std::endl;
            userDataList[i].printUserData();
            std::cout << std::endl;
        }
    }
}

void deleteUserData(std::vector<UserData>& userDataList) {
    if (userDataList.empty()) {
        std::cout << "The list is empty! Unable to delete UserData." << std::endl << std::endl;
        return;
    }

    int indexToDelete;
    std::cout << "Enter the index of UserData to delete (1 to " << userDataList.size() << "): ";
    std::cin >> indexToDelete;

    if (indexToDelete >= 1 && indexToDelete <= userDataList.size()) {
        userDataList.erase(userDataList.begin() + indexToDelete - 1);
        std::cout << "UserData at index " << indexToDelete << " deleted successfully." << std::endl << std::endl;
    } else {
        std::cout << "Invalid index! UserData does not exist at index " << indexToDelete << "." << std::endl << std::endl;
    }
}

// Function to perform radix sort on UserData entries based on angles
void radixSortAngles(std::vector<UserData>& userDataList) {
    const int numBits = 16;  // Assuming angles are represented in radians, adjust as needed
    const int numBins = 1 << numBits;

    // Temporary vectors for sorting
    std::vector<std::vector<UserData>> bins(numBins);

    // Perform radix sort on angles
    for (int bit = 0; bit < numBits; ++bit) {
        for (UserData& userData : userDataList) {
            int index = static_cast<int>((userData.angle / (2 * M_PI)) * numBins) >> bit;
            bins[index].push_back(userData);
        }

        // Combine all bins back into the original vector
        userDataList.clear();
        for (std::vector<UserData>& bin : bins) {
            userDataList.insert(userDataList.end(), bin.begin(), bin.end());
            bin.clear();
        }
    }
}

void runGeometryProgram() {
    std::vector<UserData> userDataList;

    char choice = ' ';
    while (choice != 'Q') {
        std::cout << "Please make your choice:" << std::endl
                  << "> A  to add a new UserData" << std::endl
                  << "> S  to show all UserData" << std::endl
                  << "> D  to radix sort UserData by angle" << std::endl  // Use radix sort for angle sorting
                  << "> W  to delete a UserData" << std::endl
                  << "> Q  to quit" << std::endl
                  << "-->> ";
        std::cin >> choice;
        std::cout << std::endl;

        switch (choice) {
            case 'A':
                std::cout << ">> Add new UserData" << std::endl << std::endl;
                addUserData(userDataList);
                break;
            case 'S':
                std::cout << ">> Show all UserData" << std::endl << std::endl;
                showAllUserData(userDataList);
                break;
            case 'D':
                std::cout << ">> Radix sort UserData by angle" << std::endl << std::endl;
                radixSortAngles(userDataList);
                break;
            case 'W':
                std::cout << ">> Delete UserData" << std::endl << std::endl;
                deleteUserData(userDataList);
                break;
            case 'Q':
                std::cout << ">> Quitting the program" << std::endl;
                break;
            default:
                std::cout << ">> Incorrect input, try again" << std::endl << std::endl;
                break;
        }
    }
}

int main() {
    runGeometryProgram();
    return 0;
}

