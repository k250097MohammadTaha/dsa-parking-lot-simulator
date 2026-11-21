#include <iostream>
#include <string>

using namespace std;

string toUpperCase(string str)
{
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = str[i] - 32;
        }
    }
    return str;
}

string trimSpaces(string str)
{
    int start = 0;
    while (start < (int)str.length() && str[start] == ' ')
    {
        start++;
    }
    int end = (int)str.length() - 1;
    while (end >= start && str[end] == ' ')
    {
        end--;
    }
    string result = "";
    for (int i = start; i <= end; i++)
    {
        result += str[i];
    }
    return result;
}

bool isAllDigits(string str)
{
    if (str.length() == 0)
    {
        return false;
    }
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }
    return true;
}

int stringToInt(string str)
{
    int num = 0;
    for (int i = 0; i < (int)str.length(); i++)
    {
        num = num * 10 + (str[i] - '0');
    }
    return num;
}

bool parseSmartTime(string input, int &hr, int &min)
{
    input = trimSpaces(input);
    if (input.length() == 0)
    {
        return false;
    }

    int sepIndex = -1;
    for (int i = 0; i < (int)input.length(); i++)
    {
        if (input[i] == ' ' || input[i] == ':')
        {
            sepIndex = i;
            break;
        }
    }

    if (sepIndex != -1)
    {
        string hrPart = "";
        string minPart = "";
        for (int i = 0; i < sepIndex; i++)
        {
            hrPart += input[i];
        }
        for (int i = sepIndex + 1; i < (int)input.length(); i++)
        {
            minPart += input[i];
        }
        hrPart = trimSpaces(hrPart);
        minPart = trimSpaces(minPart);

        if (!isAllDigits(hrPart) || !isAllDigits(minPart))
        {
            return false;
        }
        hr = stringToInt(hrPart);
        min = stringToInt(minPart);
    }
    else
    {
        if (!isAllDigits(input))
        {
            return false;
        }
        if (input.length() == 3 || input.length() == 4)
        {
            int combined = stringToInt(input);
            hr = combined / 100;
            min = combined % 100;
        }
        else
        {
            return false;
        }
    }

    if (hr < 0 || hr > 23 || min < 0 || min > 59)
    {
        return false;
    }
    return true;
}

void promptValidPlate(string &plate)
{
    while (true)
    {
        cout << "Enter License Plate (e.g., KHI-2026): ";
        getline(cin, plate);
        plate = toUpperCase(trimSpaces(plate));

        if (plate == "")
        {
            cout << "[INVALID INPUT] License plate cannot be empty. Please try again.\n";
            continue;
        }
        break;
    }
}

void promptValidOwner(string &owner)
{
    while (true)
    {
        cout << "Enter Driver Name: ";
        getline(cin, owner);
        owner = trimSpaces(owner);

        if (owner == "")
        {
            cout << "[INVALID INPUT] Driver name cannot be empty. Please try again.\n";
            continue;
        }
        break;
    }
}

void promptValidVehicleType(string &type)
{
    while (true)
    {
        cout << "Enter Vehicle Type (BIKE / CAR / SUV): ";
        getline(cin, type);
        type = toUpperCase(trimSpaces(type));

        if (type == "BIKE" || type == "CAR" || type == "SUV")
        {
            break;
        }
        cout << "[INVALID INPUT] Unknown vehicle type! Please enter BIKE, CAR, or SUV.\n";
    }
}

void promptValidTime(string label, int &hr, int &min)
{
    string rawTime;
    while (true)
    {
        cout << "Enter " << label << " (e.g., 09 30, 09:30, or 0930): ";
        getline(cin, rawTime);
        if (parseSmartTime(rawTime, hr, min))
        {
            break;
        }
        cout << "[INVALID TIME] Please enter a valid 24-hour time (e.g., 09 30 or 0930).\n";
    }
}

class SlotInfo
{
public:
    int slotNumber;
    bool isOccupied;
    string licensePlate;
    string vehicleType;

    SlotInfo()
    {
        slotNumber = 0;
        isOccupied = false;
        licensePlate = "";
        vehicleType = "";
    }
};

class MinHeap
{
public:
    int *heapArr;
    int capacity;
    int currentSize;

    MinHeap(int cap)
    {
        capacity = cap;
        currentSize = 0;
        heapArr = new int[capacity];
    }

    ~MinHeap()
    {
        delete[] heapArr;
    }

    int parent(int i)
    {
        return (i - 1) / 2;
    }

    int leftChild(int i)
    {
        return (2 * i) + 1;
    }

    int rightChild(int i)
    {
        return (2 * i) + 2;
    }

    void swapValues(int &a, int &b)
    {
        int temp = a;
        a = b;
        b = temp;
    }

    bool isEmpty()
    {
        return currentSize == 0;
    }

    void heapifyUp(int index)
    {
        while (index != 0 && heapArr[parent(index)] > heapArr[index])
        {
            swapValues(heapArr[index], heapArr[parent(index)]);
            index = parent(index);
        }
    }

    void heapifyDown(int index)
    {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < currentSize && heapArr[left] < heapArr[smallest])
        {
            smallest = left;
        }
        if (right < currentSize && heapArr[right] < heapArr[smallest])
        {
            smallest = right;
        }
        if (smallest != index)
        {
            swapValues(heapArr[index], heapArr[smallest]);
            heapifyDown(smallest);
        }
    }

    void insertSlot(int slotNum)
    {
        if (currentSize == capacity)
        {
            return;
        }
        int i = currentSize;
        heapArr[i] = slotNum;
        currentSize++;
        heapifyUp(i);
    }

    int extractMin()
    {
        if (currentSize <= 0)
        {
            return -1;
        }
        if (currentSize == 1)
        {
            currentSize--;
            return heapArr[0];
        }
        int root = heapArr[0];
        heapArr[0] = heapArr[currentSize - 1];
        currentSize--;
        heapifyDown(0);
        return root;
    }

    int getMin()
    {
        if (currentSize <= 0)
        {
            return -1;
        }
        return heapArr[0];
    }
};

class VehicleNode
{
public:
    string licensePlate;
    string ownerName;
    string vehicleType;
    int slotNumber;
    int entryHour;
    int entryMinute;
    VehicleNode *next;

    VehicleNode(string plate, string owner, string type, int slot, int hr, int min)
    {
        licensePlate = plate;
        ownerName = owner;
        vehicleType = type;
        slotNumber = slot;
        entryHour = hr;
        entryMinute = min;
        next = NULL;
    }
};

class HashTable
{
public:
    VehicleNode **table;
    int tableSize;
    int totalParked;

    HashTable(int size)
    {
        tableSize = size;
        totalParked = 0;
        table = new VehicleNode *[tableSize];
        for (int i = 0; i < tableSize; i++)
        {
            table[i] = NULL;
        }
    }

    ~HashTable()
    {
        clearTable();
        delete[] table;
    }

    void clearTable()
    {
        for (int i = 0; i < tableSize; i++)
        {
            VehicleNode *current = table[i];
            while (current != NULL)
            {
                VehicleNode *temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = NULL;
        }
        totalParked = 0;
    }

    int hashFunction(string key)
    {
        int hashVal = 0;
        for (int i = 0; i < (int)key.length(); i++)
        {
            hashVal = (hashVal * 31 + key[i]) % tableSize;
        }
        if (hashVal < 0)
        {
            hashVal += tableSize;
        }
        return hashVal;
    }

    void insertVehicle(string plate, string owner, string type, int slot, int hr, int min)
    {
        int index = hashFunction(plate);
        VehicleNode *newNode = new VehicleNode(plate, owner, type, slot, hr, min);
        newNode->next = table[index];
        table[index] = newNode;
        totalParked++;
    }

    VehicleNode *searchVehicle(string plate)
    {
        int index = hashFunction(plate);
        VehicleNode *current = table[index];
        while (current != NULL)
        {
            if (current->licensePlate == plate)
            {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    bool removeVehicle(string plate)
    {
        int index = hashFunction(plate);
        VehicleNode *current = table[index];
        VehicleNode *prev = NULL;

        while (current != NULL)
        {
            if (current->licensePlate == plate)
            {
                if (prev == NULL)
                {
                    table[index] = current->next;
                }
                else
                {
                    prev->next = current->next;
                }
                delete current;
                totalParked--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    void displayAllParked()
    {
        if (totalParked == 0)
        {
            cout << "\nNo vehicles are currently parked in the lot.\n";
            return;
        }
        cout << "\n--- ACTIVE PARKED VEHICLES (HASH TABLE DIRECTORY) ---\n";
        for (int i = 0; i < tableSize; i++)
        {
            VehicleNode *current = table[i];
            while (current != NULL)
            {
                cout << "Bucket [" << i << "] -> Plate: " << current->licensePlate
                     << " | Owner: " << current->ownerName
                     << " | Type: " << current->vehicleType
                     << " | Slot: #" << current->slotNumber
                     << " | Entry Time: " << (current->entryHour < 10 ? "0" : "") << current->entryHour
                     << ":" << (current->entryMinute < 10 ? "0" : "") << current->entryMinute << "\n";
                current = current->next;
            }
        }
    }
};

class QueueNode
{
public:
    string licensePlate;
    string ownerName;
    string vehicleType;
    int arrivalHour;
    int arrivalMinute;
    QueueNode *next;

    QueueNode(string plate, string owner, string type, int hr, int min)
    {
        licensePlate = plate;
        ownerName = owner;
        vehicleType = type;
        arrivalHour = hr;
        arrivalMinute = min;
        next = NULL;
    }
};

class WaitQueue
{
public:
    QueueNode *front;
    QueueNode *rear;
    int count;

    WaitQueue()
    {
        front = NULL;
        rear = NULL;
        count = 0;
    }

    ~WaitQueue()
    {
        clearQueue();
    }

    void clearQueue()
    {
        while (front != NULL)
        {
            QueueNode *temp = front;
            front = front->next;
            delete temp;
        }
        rear = NULL;
        count = 0;
    }

    bool isEmpty()
    {
        return front == NULL;
    }

    void enqueue(string plate, string owner, string type, int hr, int min)
    {
        QueueNode *newNode = new QueueNode(plate, owner, type, hr, min);
        if (rear == NULL)
        {
            front = newNode;
            rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }

    QueueNode *dequeue()
    {
        if (isEmpty())
        {
            return NULL;
        }
        QueueNode *temp = front;
        front = front->next;
        if (front == NULL)
        {
            rear = NULL;
        }
        count--;
        temp->next = NULL;
        return temp;
    }

    bool isAlreadyWaiting(string plate)
    {
        QueueNode *current = front;
        while (current != NULL)
        {
            if (current->licensePlate == plate)
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool removeSpecificVehicle(string plate)
    {
        QueueNode *current = front;
        QueueNode *prev = NULL;

        while (current != NULL)
        {
            if (current->licensePlate == plate)
            {
                if (prev == NULL)
                {
                    front = current->next;
                    if (front == NULL)
                    {
                        rear = NULL;
                    }
                }
                else
                {
                    prev->next = current->next;
                    if (current == rear)
                    {
                        rear = prev;
                    }
                }
                delete current;
                count--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    void displayQueue()
    {
        if (isEmpty())
        {
            cout << "\nWaiting Queue is currently empty.\n";
            return;
        }
        cout << "\n--- OVERFLOW WAITING QUEUE (FIFO) ---\n";
        QueueNode *current = front;
        int pos = 1;
        while (current != NULL)
        {
            cout << pos << ". Plate: " << current->licensePlate
                 << " | Owner: " << current->ownerName
                 << " | Type: " << current->vehicleType
                 << " | Arrived: " << (current->arrivalHour < 10 ? "0" : "") << current->arrivalHour
                 << ":" << (current->arrivalMinute < 10 ? "0" : "") << current->arrivalMinute << "\n";
            current = current->next;
            pos++;
        }
    }
};

class StackNode
{
public:
    string actionType;
    string licensePlate;
    string ownerName;
    string vehicleType;
    int slotNumber;
    int feePaid;
    int durationHours;
    StackNode *next;

    StackNode(string act, string plate, string owner, string type, int slot, int fee, int hrs)
    {
        actionType = act;
        licensePlate = plate;
        ownerName = owner;
        vehicleType = type;
        slotNumber = slot;
        feePaid = fee;
        durationHours = hrs;
        next = NULL;
    }
};

class ActionStack
{
public:
    StackNode *top;
    int count;

    ActionStack()
    {
        top = NULL;
        count = 0;
    }

    ~ActionStack()
    {
        clearStack();
    }

    void clearStack()
    {
        while (top != NULL)
        {
            StackNode *temp = top;
            top = top->next;
            delete temp;
        }
        count = 0;
    }

    bool isEmpty()
    {
        return top == NULL;
    }

    void push(string act, string plate, string owner, string type, int slot, int fee, int hrs)
    {
        StackNode *newNode = new StackNode(act, plate, owner, type, slot, fee, hrs);
        newNode->next = top;
        top = newNode;
        count++;
    }

    StackNode *pop()
    {
        if (isEmpty())
        {
            return NULL;
        }
        StackNode *temp = top;
        top = top->next;
        temp->next = NULL;
        count--;
        return temp;
    }

    void displayHistory()
    {
        if (isEmpty())
        {
            cout << "\nNo gate transactions recorded yet.\n";
            return;
        }
        cout << "\n--- GATE AUDIT & DEPARTURE LOG (LIFO STACK) ---\n";
        StackNode *current = top;
        int num = 1;
        while (current != NULL)
        {
            cout << num << ". [" << current->actionType << "] Plate: " << current->licensePlate
                 << " | Owner: " << current->ownerName
                 << " | Type: " << current->vehicleType
                 << " | Slot: #" << current->slotNumber;
            if (current->actionType == "CHECKOUT")
            {
                cout << " | Duration: " << current->durationHours << " hr(s)"
                     << " | Fee Paid: Rs. " << current->feePaid;
            }
            cout << "\n";
            current = current->next;
            num++;
        }
    }
};

class ParkingLotSimulator
{
public:
    int maxSlots;
    SlotInfo *slots;
    MinHeap *slotHeap;
    HashTable *activeRegistry;
    WaitQueue *overflowQueue;
    ActionStack *undoStack;
    ActionStack *auditLogStack;
    int totalRevenue;
    int totalCheckouts;

    ParkingLotSimulator(int capacity)
    {
        maxSlots = capacity;
        totalRevenue = 0;
        totalCheckouts = 0;

        slots = new SlotInfo[maxSlots + 1];
        slotHeap = new MinHeap(maxSlots);
        activeRegistry = new HashTable(maxSlots * 2 + 1);
        overflowQueue = new WaitQueue();
        undoStack = new ActionStack();
        auditLogStack = new ActionStack();

        for (int i = 1; i <= maxSlots; i++)
        {
            slots[i].slotNumber = i;
            slots[i].isOccupied = false;
            slotHeap->insertSlot(i);
        }
    }

    ~ParkingLotSimulator()
    {
        delete[] slots;
        delete slotHeap;
        delete activeRegistry;
        delete overflowQueue;
        delete undoStack;
        delete auditLogStack;
    }

    void resetLot(int newCapacity)
    {
        delete[] slots;
        delete slotHeap;
        delete activeRegistry;
        delete overflowQueue;
        delete undoStack;
        delete auditLogStack;

        maxSlots = newCapacity;
        totalRevenue = 0;
        totalCheckouts = 0;

        slots = new SlotInfo[maxSlots + 1];
        slotHeap = new MinHeap(maxSlots);
        activeRegistry = new HashTable(maxSlots * 2 + 1);
        overflowQueue = new WaitQueue();
        undoStack = new ActionStack();
        auditLogStack = new ActionStack();

        for (int i = 1; i <= maxSlots; i++)
        {
            slots[i].slotNumber = i;
            slots[i].isOccupied = false;
            slotHeap->insertSlot(i);
        }
    }

    bool isValidTime(int hr, int min)
    {
        return (hr >= 0 && hr <= 23 && min >= 0 && min <= 59);
    }

    int getHourlyRate(string type)
    {
        type = toUpperCase(type);
        if (type == "BIKE")
        {
            return 50;
        }
        if (type == "SUV" || type == "TRUCK")
        {
            return 150;
        }
        return 100;
    }

    void parkVehicle(string plate, string owner, string type, int hr, int min)
    {
        plate = toUpperCase(trimSpaces(plate));
        type = toUpperCase(trimSpaces(type));
        owner = trimSpaces(owner);
        if (owner == "")
        {
            owner = "Unknown";
        }

        if (plate == "")
        {
            cout << "[ERROR] License plate cannot be empty.\n";
            return;
        }
        if (!isValidTime(hr, min))
        {
            cout << "[ERROR] Invalid time format. Use 0-23 for hours and 0-59 for minutes.\n";
            return;
        }
        if (activeRegistry->searchVehicle(plate) != NULL)
        {
            cout << "[ERROR] Duplicate entry! Vehicle " << plate << " is already parked.\n";
            return;
        }
        if (overflowQueue->isAlreadyWaiting(plate))
        {
            cout << "[ERROR] Duplicate entry! Vehicle " << plate << " is already in the waiting queue.\n";
            return;
        }

        if (!slotHeap->isEmpty())
        {
            int nearestSlot = slotHeap->extractMin();
            slots[nearestSlot].isOccupied = true;
            slots[nearestSlot].licensePlate = plate;
            slots[nearestSlot].vehicleType = type;

            activeRegistry->insertVehicle(plate, owner, type, nearestSlot, hr, min);
            undoStack->push("PARK", plate, owner, type, nearestSlot, 0, 0);
            auditLogStack->push("CHECKIN", plate, owner, type, nearestSlot, 0, 0);

            cout << "[SUCCESS] Vehicle " << plate << " (" << type << ") assigned to nearest Slot #" << nearestSlot << ".\n";
        }
        else
        {
            overflowQueue->enqueue(plate, owner, type, hr, min);
            undoStack->push("WAIT", plate, owner, type, -1, 0, 0);
            auditLogStack->push("QUEUED", plate, owner, type, -1, 0, 0);

            cout << "[LOT FULL] All " << maxSlots << " slots occupied. Vehicle " << plate
                 << " added to Waiting Queue at position #" << overflowQueue->count << ".\n";
        }
    }

    void checkoutVehicle(string plate, int exitHr, int exitMin)
    {
        plate = toUpperCase(trimSpaces(plate));
        if (plate == "")
        {
            cout << "[ERROR] License plate cannot be empty.\n";
            return;
        }
        if (!isValidTime(exitHr, exitMin))
        {
            cout << "[ERROR] Invalid exit time.\n";
            return;
        }

        VehicleNode *record = activeRegistry->searchVehicle(plate);
        if (record == NULL)
        {
            cout << "[ERROR] Vehicle " << plate << " not found in the active parking lot.\n";
            return;
        }

        int freedSlot = record->slotNumber;
        string owner = record->ownerName;
        string type = record->vehicleType;
        int entryTotalMin = record->entryHour * 60 + record->entryMinute;
        int exitTotalMin = exitHr * 60 + exitMin;

        int diffMin = exitTotalMin - entryTotalMin;
        if (diffMin < 0)
        {
            diffMin += 24 * 60;
        }
        if (diffMin == 0)
        {
            diffMin = 60;
        }

        int billedHours = (diffMin + 59) / 60;
        int rate = getHourlyRate(type);
        int totalFee = billedHours * rate;

        activeRegistry->removeVehicle(plate);
        slots[freedSlot].isOccupied = false;
        slots[freedSlot].licensePlate = "";
        slots[freedSlot].vehicleType = "";

        totalRevenue += totalFee;
        totalCheckouts++;
        auditLogStack->push("CHECKOUT", plate, owner, type, freedSlot, totalFee, billedHours);

        cout << "\n--- RECEIPT FOR VEHICLE " << plate << " ---\n";
        cout << "Owner Name     : " << owner << "\n";
        cout << "Vehicle Type   : " << type << " (Rs. " << rate << "/hr)\n";
        cout << "Vacated Slot   : #" << freedSlot << "\n";
        cout << "Billed Duration: " << billedHours << " hour(s)\n";
        cout << "Total Fee Due  : Rs. " << totalFee << "\n";
        cout << "-------------------------------------\n";

        if (!overflowQueue->isEmpty())
        {
            QueueNode *nextCar = overflowQueue->dequeue();
            slots[freedSlot].isOccupied = true;
            slots[freedSlot].licensePlate = nextCar->licensePlate;
            slots[freedSlot].vehicleType = nextCar->vehicleType;

            activeRegistry->insertVehicle(nextCar->licensePlate, nextCar->ownerName, nextCar->vehicleType, freedSlot, exitHr, exitMin);
            undoStack->push("PARK", nextCar->licensePlate, nextCar->ownerName, nextCar->vehicleType, freedSlot, 0, 0);
            auditLogStack->push("AUTO-PARK", nextCar->licensePlate, nextCar->ownerName, nextCar->vehicleType, freedSlot, 0, 0);

            cout << "[QUEUE AUTO-ASSIGN] Vacated Slot #" << freedSlot << " automatically assigned to waiting vehicle "
                 << nextCar->licensePlate << " (" << nextCar->ownerName << ").\n";
            delete nextCar;
        }
        else
        {
            slotHeap->insertSlot(freedSlot);
            cout << "[SLOT RESTORED] Slot #" << freedSlot << " returned to Min-Heap as available.\n";
        }
    }

    void undoLastGateEntry()
    {
        while (!undoStack->isEmpty())
        {
            StackNode *lastAction = undoStack->pop();

            if (lastAction->actionType == "WAIT")
            {
                if (overflowQueue->removeSpecificVehicle(lastAction->licensePlate))
                {
                    auditLogStack->push("UNDO-WAIT", lastAction->licensePlate, lastAction->ownerName, lastAction->vehicleType, -1, 0, 0);
                    cout << "[UNDO SUCCESS] Removed vehicle " << lastAction->licensePlate << " from the waiting queue.\n";
                    delete lastAction;
                    return;
                }
            }
            else if (lastAction->actionType == "PARK")
            {
                VehicleNode *currentRecord = activeRegistry->searchVehicle(lastAction->licensePlate);
                if (currentRecord != NULL && currentRecord->slotNumber == lastAction->slotNumber)
                {
                    int slotToFree = lastAction->slotNumber;
                    activeRegistry->removeVehicle(lastAction->licensePlate);
                    slots[slotToFree].isOccupied = false;
                    slots[slotToFree].licensePlate = "";
                    slots[slotToFree].vehicleType = "";

                    auditLogStack->push("UNDO-PARK", lastAction->licensePlate, lastAction->ownerName, lastAction->vehicleType, slotToFree, 0, 0);
                    cout << "[UNDO SUCCESS] Cancelled gate entry for " << lastAction->licensePlate
                         << " and freed Slot #" << slotToFree << ".\n";

                    if (!overflowQueue->isEmpty())
                    {
                        QueueNode *nextCar = overflowQueue->dequeue();
                        slots[slotToFree].isOccupied = true;
                        slots[slotToFree].licensePlate = nextCar->licensePlate;
                        slots[slotToFree].vehicleType = nextCar->vehicleType;

                        activeRegistry->insertVehicle(nextCar->licensePlate, nextCar->ownerName, nextCar->vehicleType, slotToFree, nextCar->arrivalHour, nextCar->arrivalMinute);
                        undoStack->push("PARK", nextCar->licensePlate, nextCar->ownerName, nextCar->vehicleType, slotToFree, 0, 0);
                        cout << "[QUEUE AUTO-ASSIGN] Freed Slot #" << slotToFree << " assigned to waiting vehicle " << nextCar->licensePlate << ".\n";
                        delete nextCar;
                    }
                    else
                    {
                        slotHeap->insertSlot(slotToFree);
                    }

                    delete lastAction;
                    return;
                }
            }
            delete lastAction;
        }
        cout << "[UNDO NOTICE] No active check-in actions available to undo.\n";
    }

    void searchVehicleByPlate(string plate)
    {
        plate = toUpperCase(trimSpaces(plate));
        VehicleNode *found = activeRegistry->searchVehicle(plate);
        if (found != NULL)
        {
            cout << "\n[VEHICLE LOCATED IN PARKING LOT]\n";
            cout << "License Plate : " << found->licensePlate << "\n";
            cout << "Owner Name    : " << found->ownerName << "\n";
            cout << "Vehicle Type  : " << found->vehicleType << "\n";
            cout << "Assigned Slot : #" << found->slotNumber << " (Distance " << found->slotNumber << " units from entrance)\n";
            cout << "Check-In Time : " << (found->entryHour < 10 ? "0" : "") << found->entryHour
                 << ":" << (found->entryMinute < 10 ? "0" : "") << found->entryMinute << "\n";
            return;
        }
        if (overflowQueue->isAlreadyWaiting(plate))
        {
            cout << "\n[VEHICLE IN WAITING QUEUE] Vehicle " << plate << " is currently waiting for an empty slot.\n";
            return;
        }
        cout << "\n[NOT FOUND] Vehicle " << plate << " is neither parked nor in the waiting queue.\n";
    }

    void displayParkingGrid()
    {
        cout << "\n================ LIVE PARKING LOT MAP ================\n";
        cout << "Total Capacity: " << maxSlots
             << " | Occupied: " << activeRegistry->totalParked
             << " | Free: " << slotHeap->currentSize
             << " | Waiting Queue: " << overflowQueue->count << "\n";
        if (!slotHeap->isEmpty())
        {
            cout << "Next Nearest Slot (Min-Heap Top): Slot #" << slotHeap->getMin() << "\n";
        }
        else
        {
            cout << "Next Nearest Slot (Min-Heap Top): NONE (LOT FULL)\n";
        }
        cout << "------------------------------------------------------\n";

        for (int i = 1; i <= maxSlots; i++)
        {
            cout << "Slot #" << (i < 10 ? "0" : "") << i << ": ";
            if (slots[i].isOccupied)
            {
                cout << "[ " << slots[i].licensePlate << " | " << slots[i].vehicleType << " ]\t";
            }
            else
            {
                cout << "[    FREE    ]\t";
            }
            if (i % 3 == 0 || i == maxSlots)
            {
                cout << "\n";
            }
        }
        cout << "======================================================\n";
    }

    void displaySummaryStats()
    {
        cout << "\n================ REVENUE & SYSTEM SUMMARY ================\n";
        cout << "Total Vehicles Currently Parked : " << activeRegistry->totalParked << "\n";
        cout << "Total Vehicles in Waiting Line  : " << overflowQueue->count << "\n";
        cout << "Total Vehicles Checked Out      : " << totalCheckouts << "\n";
        cout << "Total Parking Revenue Collected : Rs. " << totalRevenue << "\n";
        cout << "==========================================================\n";
        auditLogStack->displayHistory();
    }

    void runAutomatedTestSuite()
    {
        cout << "\n************************************************************\n";
        cout << "   RUNNING AUTOMATED NORMAL & EDGE-CASE TEST SUITE\n";
        cout << "************************************************************\n";

        resetLot(3);
        cout << "\n[TEST 1: Empty Lot Checkout & Undo Edge Cases]\n";
        checkoutVehicle("KHI-9999", 10, 0);
        undoLastGateEntry();

        cout << "\n[TEST 2: Invalid Input & Lowercase Normalization]\n";
        parkVehicle("", "Ali", "Car", 9, 0);
        parkVehicle("abc-123", "Ali", "car", 25, 70);

        cout << "\n[TEST 3: Normal Nearest-Slot Allocation via Min-Heap (3-Slot Lot)]\n";
        parkVehicle("khi-1001", "Taha", "car", 8, 30);
        parkVehicle("KHI-1002", "Haseeb", "bike", 8, 45);
        parkVehicle("KHI-1003", "Bilal", "suv", 9, 0);
        displayParkingGrid();

        cout << "\n[TEST 4: Duplicate License Plate Check (Case-Insensitive)]\n";
        parkVehicle("KHI-1001", "Taha", "CAR", 9, 10);

        cout << "\n[TEST 5: Overflow Waiting Queue When Lot is Full]\n";
        parkVehicle("KHI-2001", "Usman", "Car", 9, 15);
        parkVehicle("KHI-2002", "Zain", "Bike", 9, 20);
        overflowQueue->displayQueue();

        cout << "\n[TEST 6: Undo Gate Entry from Waiting Queue]\n";
        undoLastGateEntry();
        overflowQueue->displayQueue();

        cout << "\n[TEST 7: Middle-Slot Checkout & Automatic Queue Assignment]\n";
        checkoutVehicle("khi-1002", 11, 15);
        displayParkingGrid();

        cout << "\n[TEST 8: Out-of-Order Checkout & Min-Heap Nearest Slot Restoration]\n";
        checkoutVehicle("KHI-1003", 12, 0);
        checkoutVehicle("KHI-1001", 12, 30);
        displayParkingGrid();
        parkVehicle("KHI-3001", "Hamza", "Car", 13, 0);
        displayParkingGrid();

        displaySummaryStats();
        cout << "\n**************** ALL TESTS COMPLETED ****************\n";
    }
};

int main()
{
    int initialCapacity = 6;
    cout << "======================================================\n";
    cout << "         SMART CAMPUS PARKING LOT SIMULATOR           \n";
    cout << "======================================================\n";
    cout << "Enter total number of parking slots to initialize: ";
    if (!(cin >> initialCapacity) || initialCapacity <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        initialCapacity = 6;
        cout << "Invalid input. Defaulting to 6 slots.\n";
    }
    else
    {
        cin.ignore(1000, '\n');
    }

    ParkingLotSimulator simulator(initialCapacity);

    int choice = -1;
    while (choice != 0)
    {
        cout << "\n---------------- MAIN MENU ----------------\n";
        cout << "1. Park a Vehicle (Min-Heap / Overflow Queue)\n";
        cout << "2. Checkout a Vehicle & Generate Bill (Hash Table)\n";
        cout << "3. Search Vehicle by License Plate (O(1) Lookup)\n";
        cout << "4. Undo Last Gate Check-In (LIFO Stack)\n";
        cout << "5. View Live Parking Lot Map\n";
        cout << "6. View Active Hash Table Directory\n";
        cout << "7. View Overflow Waiting Queue\n";
        cout << "8. View Gate Audit Log & Total Revenue\n";
        cout << "9. Run Automated Edge-Case Test Suite\n";
        cout << "0. Exit Simulator\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[ERROR] Please enter a valid numeric choice.\n";
            continue;
        }
        cin.ignore(1000, '\n');

        if (choice == 1)
        {
            string plate, owner, type;
            int hr, min;
            promptValidPlate(plate);
            promptValidOwner(owner);
            promptValidVehicleType(type);
            promptValidTime("Entry Time", hr, min);
            simulator.parkVehicle(plate, owner, type, hr, min);
        }
        else if (choice == 2)
        {
            string plate;
            int hr, min;
            promptValidPlate(plate);
            promptValidTime("Exit Time", hr, min);
            simulator.checkoutVehicle(plate, hr, min);
        }
        else if (choice == 3)
        {
            string plate;
            promptValidPlate(plate);
            simulator.searchVehicleByPlate(plate);
        }
        else if (choice == 4)
        {
            simulator.undoLastGateEntry();
        }
        else if (choice == 5)
        {
            simulator.displayParkingGrid();
        }
        else if (choice == 6)
        {
            simulator.activeRegistry->displayAllParked();
        }
        else if (choice == 7)
        {
            simulator.overflowQueue->displayQueue();
        }
        else if (choice == 8)
        {
            simulator.displaySummaryStats();
        }
        else if (choice == 9)
        {
            simulator.runAutomatedTestSuite();
        }
        else if (choice == 0)
        {
            cout << "Shutting down simulator and freeing all dynamic memory...\n";
        }
        else
        {
            cout << "[ERROR] Invalid menu option.\n";
        }
    }

    return 0;
}