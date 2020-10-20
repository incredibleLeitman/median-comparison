#include <iostream>
#include <algorithm> // std::nth_element

#include "common.h"
#include "fileHandler.h"
#include "Timing.h"
#include "MedianQuicksort.h"
#include "MedianOfMedians.h"
#include "RandomizedSelect.h"
#include "Wirth.h"

int main(int argc, char** argv)
{
    // read test values from input file
    std::string type = "init\t\t\t\t\t\t";
    Timing::getInstance()->startRecord(type);
    //std::vector<uint32_t > numbers = readFromFile("testdata_mini");
    std::vector<uint32_t > numbers = readFromFile("testdata");
    Timing::getInstance()->stopRecord(type);

    // index of median is exactly middle for odd datasets, not supported for even :p
    if (numbers.size() % 2 == 0)
    {
        // TODO: just use the next element? calc arithmetic mean?
        std::cout << "TODO: define how to handle even datasets" << std::endl;
        return 1;
    }
    uint32_t idxMed = (uint32_t)(numbers.size() - 1) / 2;
    std::cout << "calculating median on index " << idxMed << " of " << numbers.size() << " elems..." << std::endl;
    print(numbers);

    // create an array from vector for algorithms using array params
    uint32_t* array = new uint32_t[numbers.size()];

    // vollständige Sortierung mit Quicksort und Ausgabe des mittleren Elements
    type = "quicksort median\t\t\t\t";
    Timing::getInstance()->startRecord(type);
    std::cout << type << getQuicksortMedian(numbers, idxMed) << std::endl;
    Timing::getInstance()->stopRecord(type);

    // comparison of std::qsort and std::sort
        // using std quicksort for array
        type = "array quicksort\t\t\t\t\t";
        std::copy(numbers.begin(), numbers.end(), array);
        Timing::getInstance()->startRecord(type);
        std::qsort(array, numbers.size(), sizeof(uint32_t), compare);
        std::cout << type << array[idxMed] << std::endl;
        Timing::getInstance()->stopRecord(type);

        // comparison "quick" sort with std::sort (introsort = hybrid quicksort + heapsort)
        // faster because C++’s templates generate optimized code for a particular data type and a particular comparison function
        type = "array std sort\t\t\t\t\t";
        std::copy(numbers.begin(), numbers.end(), array);
        Timing::getInstance()->startRecord(type);
        std::sort(array, array + numbers.size());
        std::cout << type << array[idxMed] << std::endl;
        Timing::getInstance()->stopRecord(type);

    // vorgestellter Randomzized - Select rekursiv implementiert
    type = "randomized select\t\t\t\t";
    Timing::getInstance()->startRecord(type);
    std::cout << type << getRandomizedSelectMedian(numbers, 0, (int)numbers.size() - 1, idxMed + 1) << std::endl;
    Timing::getInstance()->stopRecord(type);

    // ein weiterer Median - Algorithmus aus der Literatur - implemented with std::vector
    type = "vector median of medians only (approximative)\t";
    Timing::getInstance()->startRecord(type);
    std::cout << type << getPivotOfMedians(numbers, 0, (int)numbers.size() - 1) << std::endl;
    Timing::getInstance()->stopRecord(type);

    // ein weiterer Median - Algorithmus aus der Literatur - implemented with std::vector
    type = "vector median of medians + quickselect (exact)\t";
    Timing::getInstance()->startRecord(type);
    std::cout << type << getMedianOfMedians(numbers, 0, (int)numbers.size() - 1, idxMed) << std::endl;
    Timing::getInstance()->stopRecord(type);

    // noch ein ein weiterer Median - Algorithmus weil wir so cool sind
    type = "wirth kth element\t\t\t\t";
    Timing::getInstance()->startRecord(type);
    std::cout << type << getWirthKthSmallest(numbers, idxMed) << std::endl;
    Timing::getInstance()->stopRecord(type);

    // Verwendung des C++ STL function templates nth_element
    type = "nth element\t\t\t\t\t";
    Timing::getInstance()->startRecord(type);
    std::nth_element(numbers.begin(), numbers.begin() + idxMed, numbers.end());
    std::cout << type << numbers[idxMed] << std::endl;
    Timing::getInstance()->stopRecord(type);
    print(numbers);

    Timing::getInstance()->print();
    return 0;
}