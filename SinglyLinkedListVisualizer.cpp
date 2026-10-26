#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
using namespace std;

// Singly Linked List Visualization System
// Positions in this program are 1-based: the first node is position 1.
struct Node {
    int data;
    Node* next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

class SinglyLinkedList {
    Node* head = nullptr;
    int count = 0;

    Node* at(int position) const {
        Node* current = head;
        for (int i = 1; current && i < position; ++i) current = current->next;
        return current;
    }

    string name(int position) const { return "Node" + to_string(position); }
    string existingName(int position) const {
        return position >= 1 && position <= count ? name(position) : "NULL";
    }

public:
    ~SinglyLinkedList() { clear(); }

    int size() const { return count; }
    bool valid(int position) const { return position >= 1 && position <= count; }
    bool validInsert(int position) const { return position >= 1 && position <= count + 1; }

    void clear() {
        while (head) {
            Node* old = head;
            head = head->next;
            delete old;
        }
        count = 0;
    }

    void insert(int value, int position) {
        if (position == 1) {
            Node* newNode = new Node(value);
            newNode->next = head;
            head = newNode;
        } else {
            Node* previous = at(position - 1);
            Node* newNode = new Node(value);
            newNode->next = previous->next;
            previous->next = newNode;
        }
        ++count;
    }

    bool erase(int position, int& removedValue) {
        if (!valid(position)) return false;
        Node* removed;
        if (position == 1) {
            removed = head;
            head = head->next;
        } else {
            Node* previous = at(position - 1);
            removed = previous->next;
            previous->next = removed->next;
        }
        removedValue = removed->data;
        delete removed;
        --count;
        return true;
    }

    vector<int> find(int value) const {
        vector<int> positions;
        Node* current = head;
        int position = 1;
        while (current) {
            if (current->data == value) positions.push_back(position);
            current = current->next;
            ++position;
        }
        return positions;
    }

    bool update(int position, int newValue, int& oldValue) {
        if (!valid(position)) return false;
        Node* target = at(position);
        oldValue = target->data;
        target->data = newValue;
        return true;
    }

    void chain() const {
        cout << "Pointer chain: HEAD";
        Node* current = head;
        int position = 1;
        while (current) {
            cout << " -> [" << name(position) << " | data=" << current->data << "]";
            current = current->next;
            ++position;
        }
        cout << " -> NULL\n";
    }

    void boxes() const {
        cout << "\n================ LINKED LIST VISUALIZATION ================\n";
        if (!head) {
            cout << "HEAD -> NULL\n(The list is empty.)\n";
            cout << "============================================================\n";
            return;
        }

        const int width = 22;
        const string gap = "     ";
        vector<string> labels, data, links;
        Node* current = head;
        int position = 1;
        while (current) {
            labels.push_back(name(position));
            data.push_back("data: " + to_string(current->data));
            links.push_back(current->next ? "next: " + name(position + 1) : "next: NULL");
            current = current->next;
            ++position;
        }

        auto row = [&](const vector<string>& values, bool arrow) {
            for (size_t i = 0; i < values.size(); ++i) {
                cout << "| " << left << setw(width - 2) << values[i] << " |";
                if (i + 1 < values.size()) cout << (arrow ? " -> " : gap);
            }
            cout << '\n';
        };

        string border;
        for (size_t i = 0; i < labels.size(); ++i) {
            border += "+" + string(width, '-');
            if (i + 1 < labels.size()) border += gap;
        }
        cout << "HEAD\n  |\n  v\n" << border << '\n';
        row(labels, true);
        row(data, false);
        row(links, false);
        cout << border << '\nNULL is the end of the list.\n";
        cout << "============================================================\n";
    }

    void insertTrace(int value, int position) const {
        cout << "\n---------------- INSERTION STEP TRACE ----------------\n";
        cout << "Goal: insert " << value << " at position " << position << ".\n";
        chain();
        if (position == 1) {
            cout << "1. Create newNode(" << value << ").\n"
                 << "2. newNode->next = HEAD.\n"
                 << "3. HEAD = newNode.\n";
        } else {
            cout << "1. Move current from HEAD to " << name(position - 1) << ".\n"
                 << "2. Create newNode(" << value << ").\n"
                 << "3. newNode->next = " << existingName(position) << ".\n"
                 << "4. " << name(position - 1) << "->next = newNode.\n";
        }
        cout << "---------------------------------------------------------\n";
    }

    void deleteTrace(int position) const {
        cout << "\n---------------- DELETION STEP TRACE -------------------\n";
        cout << "Goal: delete " << name(position) << ".\n";
        chain();
        if (position == 1) {
            cout << "1. target = HEAD (" << name(1) << ").\n"
                 << "2. HEAD = target->next (" << existingName(2) << ").\n"
                 << "3. Delete target.\n";
        } else {
            cout << "1. previous = " << name(position - 1) << ".\n"
                 << "2. target = previous->next (" << name(position) << ").\n"
                 << "3. previous->next = target->next (" << existingName(position + 1) << ").\n"
                 << "4. Delete target.\n";
        }
        cout << "---------------------------------------------------------\n";
    }

    void searchTrace(int value) const {
        cout << "\n---------------- SEARCH STEP TRACE ---------------------\n";
        cout << "Looking for " << value << ": HEAD";
        Node* current = head;
        int position = 1;
        while (current) {
            cout << " -> [" << name(position) << ": " << current->data << "]"
                 << (current->data == value ? " MATCH" : " visit");
            current = current->next;
            ++position;
        }
        cout << " -> NULL\nAt each arrow: current = current->next.\n";
        cout << "---------------------------------------------------------\n";
    }

    void updateTrace(int position, int newValue) const {
        cout << "\n---------------- UPDATE STEP TRACE ---------------------\n";
        cout << "Goal: update " << name(position) << " to " << newValue << ".\n";
        chain();
        cout << "1. Start current at HEAD.\n"
             << "2. Move current until it reaches " << name(position) << ".\n"
             << "3. current->data = " << newValue << ".\n"
             << "   The next links do not change.\n"
             << "---------------------------------------------------------\n";
    }

    void traversalTrace() const {
        cout << "\n---------------- TRAVERSAL STEP TRACE ------------------\n";
        cout << "Visit order: HEAD";
        Node* current = head;
        int position = 1;
        while (current) {
            cout << " -> " << name(position) << "(data=" << current->data << ")";
            current = current->next;
            ++position;
        }
        cout << " -> NULL\n"
             << "At every step: visit current, then current = current->next.\n"
             << "Traversal stops when current == NULL.\n"
             << "---------------------------------------------------------\n";
    }
};

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void menu() {
    cout << "\n==================== MAIN MENU ====================\n"
         << "1. Insert at beginning\n2. Insert at end\n3. Insert at position\n"
         << "4. Delete by position\n5. Delete by value (first match)\n"
         << "6. Search by value\n7. Update by position\n8. Traverse list\n"
         << "9. Display visualization\n0. Exit\n"
         << "===================================================\n";
}

void showAfterChange(const SinglyLinkedList& list) {
    cout << "\nCurrent list after the operation:";
    list.boxes();
}

int main() {
    SinglyLinkedList list;
    cout << "SINGLY LINKED LIST VISUALIZATION SYSTEM\n"
         << "========================================\nAll positions are 1-based.\n";
    list.boxes();

    while (true) {
        menu();
        int choice = readInt("Enter your choice: ");
        switch (choice) {
        case 1: {
            int value = readInt("Enter value to insert: ");
            list.insertTrace(value, 1);
            list.insert(value, 1);
            cout << "Inserted " << value << " at the beginning.\n";
            showAfterChange(list);
            break;
        }
        case 2: {
            int value = readInt("Enter value to insert: ");
            int position = list.size() + 1;
            list.insertTrace(value, position);
            list.insert(value, position);
            cout << "Inserted " << value << " at the end.\n";
            showAfterChange(list);
            break;
        }
        case 3: {
            int position = readInt("Enter position (1 to " + to_string(list.size() + 1) + "): ");
            int value = readInt("Enter value to insert: ");
            if (!list.validInsert(position)) {
                cout << "Invalid position. No node was inserted.\n";
                break;
            }
            list.insertTrace(value, position);
            list.insert(value, position);
            cout << "Inserted " << value << " at position " << position << ".\n";
            showAfterChange(list);
            break;
        }
        case 4: {
            int position = readInt("Enter position to delete: ");
            if (!list.valid(position)) {
                cout << "Invalid position. No node was deleted.\n";
                break;
            }
            list.deleteTrace(position);
            int removed = 0;
            list.erase(position, removed);
            cout << "Deleted value " << removed << " from position " << position << ".\n";
            showAfterChange(list);
            break;
        }
        case 5: {
            int value = readInt("Enter value to delete: ");
            vector<int> matches = list.find(value);
            if (matches.empty()) {
                cout << value << " was not found.\n";
                list.searchTrace(value);
                break;
            }
            list.deleteTrace(matches.front());
            int removed = 0;
            list.erase(matches.front(), removed);
            cout << "Deleted the first " << value << " at position " << matches.front() << ".\n";
            showAfterChange(list);
            break;
        }
        case 6: {
            int value = readInt("Enter value to search: ");
            list.searchTrace(value);
            vector<int> matches = list.find(value);
            if (matches.empty()) {
                cout << value << " was not found.\n";
            } else {
                cout << value << " found at position(s): ";
                for (size_t i = 0; i < matches.size(); ++i)
                    cout << (i ? ", " : "") << matches[i];
                cout << ".\n";
            }
            list.boxes();
            break;
        }
        case 7: {
            int position = readInt("Enter position to update: ");
            int newValue = readInt("Enter new value: ");
            if (!list.valid(position)) {
                cout << "Invalid position. No node was updated.\n";
                break;
            }
            list.updateTrace(position, newValue);
            int oldValue = 0;
            list.update(position, newValue, oldValue);
            cout << "Updated position " << position << " from " << oldValue << " to " << newValue << ".\n";
            showAfterChange(list);
            break;
        }
        case 8:
            list.traversalTrace();
            list.chain();
            list.boxes();
            break;
        case 9:
            list.chain();
            list.boxes();
            break;
        case 0:
            cout << "Thank you for using the Singly Linked List Visualization System.\n";
            return 0;
        default:
            cout << "Invalid choice. Select an option from the menu.\n";
        }
    }
}
