#include <bits/stdc++.h>

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

// Classes
class HuffmanNode {
public:
    char data;
    unsigned frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

class Compare {
public:
    bool operator()(HuffmanNode* l, HuffmanNode* r) { return l->frequency > r->frequency; }
};

class HuffmanEncoding {
public:
    static HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& freq) {
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

        for (const auto& pair : freq) {
            minHeap.push(new HuffmanNode(pair.first, pair.second));
        }

        while (minHeap.size() != 1) {
            HuffmanNode* left = minHeap.top();
            minHeap.pop();

            HuffmanNode* right = minHeap.top();
            minHeap.pop();

            HuffmanNode* newNode = new HuffmanNode('$', left->frequency + right->frequency);
            newNode->left = left;
            newNode->right = right;

            minHeap.push(newNode);
        }

        return minHeap.top();
    }

    static void encode(HuffmanNode* root, string str, unordered_map<char, string>& huffmanCode) {
        if (root == nullptr) {
            return;
        }

        if (!root->left && !root->right) {
            huffmanCode[root->data] = str;
        }

        encode(root->left, str + "0", huffmanCode);
        encode(root->right, str + "1", huffmanCode);
    }

    static string encodeString(const string& text, const unordered_map<char, string>& huffmanCode) {
        string encodedText = "";
        for (const char& c : text) {
            encodedText += huffmanCode.at(c);
        }
        return encodedText;
    }

    static string decode(HuffmanNode* root, const string& encodedText) {
        string decodedText = "";
        HuffmanNode* current = root;
        for (const char& bit : encodedText) {
            if (bit == '0') {
                current = current->left;
            } else {
                current = current->right;
            }

            if (!current->left && !current->right) {
                decodedText += current->data;
                current = root;
            }
        }
        return decodedText;
    }

    static void deleteTree(HuffmanNode* root) {
        if (root) {
            deleteTree(root->left);
            deleteTree(root->right);
            delete root;
        }
    }
};

class products {
protected:
    string name;
    int retailPrice;
    int sellingPrice;

public:
    products(string n, int rp, int sp) : name(n), retailPrice(rp), sellingPrice(sp) {}

    string getName() const {
        return name;
    }

    int getRetailPrice() const {
        return retailPrice;
    }

    int getSellingPrice() const {
        return sellingPrice;
    }

    int getEstimatedProfit() const {
        return sellingPrice - retailPrice;
    }
};

class paikariMarket : public products {
public:
    string nameOfBazar;
    string address;

    paikariMarket(string n, int rp, int sp, string bazar, string addr) : products(n, rp, sp), nameOfBazar(bazar), address(addr) {}

    string getMarketName() const {
        return nameOfBazar;
    }

    string getAddress() const {
        return address;
    }
};

class User {
private:
    string username;
    string encodedPassword; // Storing encoded password
    string address;

public:
    User(const string& username, const string& password, const string& address) : username(username), address(address) {
        // Huffman encoding of the password
        unordered_map<char, int> freq;
        for (char c : password) {
            freq[c]++;
        }

        HuffmanNode* root = HuffmanEncoding::buildHuffmanTree(freq);

        unordered_map<char, string> huffmanCode;
        HuffmanEncoding::encode(root, "", huffmanCode);

        encodedPassword = HuffmanEncoding::encodeString(password, huffmanCode);

        HuffmanEncoding::deleteTree(root);
    }

    const string& getUsername() const {
        return username;
    }

    const string& getAddress() const {
        return address;
    }

    const string& getEncodedPassword() const {
        return encodedPassword;
    }

    bool verifyPassword(const string& inputPassword) {
        // Implement decoding and matching logic here
        unordered_map<char, int> freq;
        for (char c : inputPassword) {
            freq[c]++;
        }

        HuffmanNode* root = HuffmanEncoding::buildHuffmanTree(freq);

        unordered_map<char, string> huffmanCode;
        HuffmanEncoding::encode(root, "", huffmanCode);

        string decodedPassword = HuffmanEncoding::decode(root, encodedPassword);

        HuffmanEncoding::deleteTree(root);

        return inputPassword == decodedPassword;
    }
};

class districts {
public:
    string name;

    districts(const string& n) : name(n) {}

    string getName(){
        return name;
    }
};

// Initial Variables
vector<User> users;
string loggedInUser = "";
string loggedInUserAddress = "Dhaka";
bool isLoggedIn = true;

unordered_map<products*, vector<paikariMarket*>> productGraph;
unordered_map<paikariMarket*, vector<products*>> marketGraph;
unordered_map<string, districts*> districtMap;
unordered_map<districts*, unordered_map<districts*, int>> districtGraph;

// function initialization
void addEdge(districts* from, districts* to,int weight);
void initializeDistricts();
void dfsTraversal(districts* currentDistrict, unordered_set<districts*>& visited);
void displayAllEdgesWithWeights();
void displayAllPlaces();
void addConnection(products* product, paikariMarket* market);
void loginUser(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketCap);
void registerUser(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketCap);
void userOptions(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketCap);
void loginUser(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketMap);
void displayAllProducts(const unordered_map<string, products*>& productMap);
pair<vector<districts*>, int> shortestPath(districts* start, districts* end);
void showShortestPath(districts* startDistrict, districts* endDistrict);
void displayAllMarkets(const unordered_map<string, vector<paikariMarket*>>& productMarketMap);
void showDistributeCost(districts* start);
vector<string> getProductSuggestion(const unordered_map<string, products*>& productMap, int budget);
void suggestProduct(const unordered_map<string, products*>& productMap);
void generateCombinations(const unordered_map<string, products*>& productMap, vector<pair<string, int>>& combination, int budget, int currentBudget, int index);
void suggestProductCombinations(const unordered_map<string, products*>& productMap);
void initialOptions(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketMap);
void calculateShippingCost();
void userOptions(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketMap);
void adminOptions(unordered_map<string, products*>&productMap, unordered_map<string, vector<paikariMarket*>>& productMarketMap);
void adminLogin(unordered_map<string, products*>&productMap, unordered_map<string, vector<paikariMarket*>>& productMarketMap);

void mainText(string s){
    cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t " << s << " \n";
        cout << "-------------------------------------\n";
}


// Original Functions

void addEdge(districts* from, districts* to, int weight) {
    districtGraph[from][to] = weight;
    districtGraph[to][from] = weight; 
}

void initializeDistricts() {
    string districtNames[] = {
        "Bagerhat", "Bandarban", "Barguna", "Barishal", "Bhola", "Bogra", "Brahmanbaria", "Chandpur", "Chapainawabganj", "Chattogram",
        "Chuadanga", "Cox's Bazar", "Cumilla", "Dhaka", "Dinajpur", "Faridpur", "Feni", "Gaibandha", "Gazipur", "Gopalganj",
        "Habiganj", "Jamalpur", "Jashore", "Jhalokati", "Jhenaidah", "Joypurhat", "Khagrachari", "Khulna", "Kishoreganj", "Kurigram",
        "Kushtia", "Lakshmipur", "Lalmonirhat", "Madaripur", "Magura", "Manikganj", "Meherpur", "Moulvibazar", "Munshiganj", "Mymensingh",
        "Naogaon", "Narail", "Narayanganj", "Narsingdi", "Natore", "Netrokona", "Nilphamari", "Noakhali", "Pabna", "Panchagarh",
        "Patuakhali", "Pirojpur", "Rajbari", "Rajshahi", "Rangamati", "Rangpur", "Satkhira", "Shariatpur", "Sherpur", "Sirajganj",
        "Sunamganj", "Sylhet", "Tangail", "Thakurgaon"
    };

    for (const auto& district : districtNames) {
        districtMap[district] = new districts(district);
    }

    addEdge(districtMap["Dhaka"], districtMap["Jamalpur"], 100);
    addEdge(districtMap["Mymensingh"], districtMap["Jamalpur"], 80);
    addEdge(districtMap["Mymensingh"], districtMap["Dhaka"], 60);
    addEdge(districtMap["Sherpur"], districtMap["Jamalpur"], 90);
    addEdge(districtMap["Dhaka"], districtMap["Tangail"], 50);
    addEdge(districtMap["Jamalpur"], districtMap["Tangail"], 30);
    addEdge(districtMap["Chittagong"], districtMap["Cox's Bazar"], 120);
addEdge(districtMap["Chittagong"], districtMap["Sylhet"], 200);
addEdge(districtMap["Rangpur"], districtMap["Rajshahi"], 150);
addEdge(districtMap["Barishal"], districtMap["Khulna"], 180);
addEdge(districtMap["Dhaka"], districtMap["Rajshahi"], 250);
// addEdge(districtMap["Mymensingh"], districtMap["Sylhet"], 220);
addEdge(districtMap["Dhaka"], districtMap["Barishal"], 280);
// addEdge(districtMap["Khulna"], districtMap["Cox's Bazar"], 320);
// addEdge(districtMap["Rangpur"], districtMap["Sylhet"], 290);
addEdge(districtMap["Rajshahi"], districtMap["Barishal"], 340);
addEdge(districtMap["Dhaka"], districtMap["Khulna"], 300);
addEdge(districtMap["Sylhet"], districtMap["Cox's Bazar"], 350);
// addEdge(districtMap["Rajshahi"], districtMap["Chittagong"], 400);
addEdge(districtMap["Barishal"], districtMap["Rangpur"], 420);
addEdge(districtMap["Mymensingh"], districtMap["Rangpur"], 260);
// addEdge(districtMap["Tangail"], districtMap["Sylhet"], 280);
// addEdge(districtMap["Cox's Bazar"], districtMap["Khulna"], 330);
// addEdge(districtMap["Jamalpur"], districtMap["Rajshahi"], 380);
// addEdge(districtMap["Sherpur"], districtMap["Barishal"], 410);
// addEdge(districtMap["Mymensingh"], districtMap["Tangail"], 90);

}

void dfsTraversal(districts* currentDistrict, unordered_set<districts*>& visited) {
    visited.insert(currentDistrict);

    for (const auto& neighbor : districtGraph[currentDistrict]) {
        districts* nextDistrict = neighbor.first;
        int weight = neighbor.second;

        if (visited.find(nextDistrict) == visited.end()) {
            cout << currentDistrict->name << " --(" << weight << ")--> " << nextDistrict->name << endl;
            dfsTraversal(nextDistrict, visited);
        }
    }
}

void displayAllEdgesWithWeights() {
    unordered_set<districts*> visited;

    for (const auto& district : districtMap) {
        if (visited.find(district.second) == visited.end()) {
            dfsTraversal(district.second, visited);
        }
    }

    char op;
    cout << "Press b to go back: ";
    cin >> op;
    clearScreen();
}

void displayAllPlaces() {
    cout << "All Places:\n";
    unordered_map<districts*, bool> visited;

    for (const auto& pair : districtMap) {
        visited[pair.second] = false;
    }

    for (const auto& pair : districtMap) {
        districts* currentDistrict = pair.second;
        if (!visited[currentDistrict]) {
            stack<districts*> stk;
            stk.push(currentDistrict);
            visited[currentDistrict] = true;

            while (!stk.empty()) {
                districts* currDistrict = stk.top();
                stk.pop();
                cout << currDistrict->name << endl;
            }
        }
    }
}

void addConnection(products* product, paikariMarket* market) {
    productGraph[product].push_back(market);
    marketGraph[market].push_back(product);
}



void registerUser(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketCap) {
    string username, password, address;
    mainText("Registration");
    cout << "Enter username: ";
    cin >> username;

    // Check if the username already exists
    auto existingUser = find_if(users.begin(), users.end(), [&username](const User& u) {
        return u.getUsername() == username;
    });

    if (existingUser != users.end()) {
        cout << "Username already exists! Try again." << endl;
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    cout << "Enter address: ";
    cin.ignore();
    getline(cin, address);

    // Register the user
    users.emplace_back(username, password, address);
    cout << "Registration successful!" << endl;
    _sleep(1000);
    clearScreen();
    loginUser(productMap, productMarketCap);
}


void loginUser(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketMap) {
    string username, password;
    mainText("User Login");
    cout << "Enter username: ";
    cin >> username;

    // Check if the user exists
    auto foundUser = find_if(users.begin(), users.end(), [&username](const User& u) {
        return u.getUsername() == username;
    });

    if (foundUser == users.end()) {
        cout << "User not found! Please register." << endl;
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    // Verify password
    if (foundUser->verifyPassword(password)) {
        isLoggedIn = true; // Set login status to true
        loggedInUser = username;
        loggedInUserAddress = foundUser->getAddress();
        cout << "Login successful, welcome " << username << "!" << endl;
        _sleep(1000);
        clearScreen();
        userOptions(productMap, productMarketMap);
    } else {
        cout << "Incorrect password! Try again." << endl;
    }
}

void displayAllProducts(const unordered_map<string, products*>& productMap) {
    mainText("Product Info");
    unordered_map<products*, bool> visited;

    for (const auto& pair : productGraph) {
        visited[pair.first] = false;
    }

    for (const auto& pair : productGraph) {
        products* product = pair.first;
        if (!visited[product]) {
            queue<products*> q;
            q.push(product);
            visited[product] = true;

            while (!q.empty()) {
                products* currProduct = q.front();
                q.pop();
                cout << "Name: " << currProduct->getName() << "\nRetail Price: $" << currProduct->getRetailPrice()
             << "\nSelling Price: $" << currProduct->getSellingPrice()
             << "\nEstimated Profit: $" << currProduct->getEstimatedProfit() << "\n\n";

                for (paikariMarket* market : productGraph[currProduct]) {
                    for (products* connectedProduct : marketGraph[market]) {
                        if (!visited[connectedProduct]) {
                            q.push(connectedProduct);
                            visited[connectedProduct] = true;
                        }
                    }
                }
            }
        }
    }
    cout << "Press 'B' to go back...";
    char back;
    cin >> back;
    clearScreen();
}

pair<vector<districts*>, int> shortestPath(districts* start, districts* end) {
    unordered_map<districts*, int> distance;
    unordered_map<districts*, districts*> parent;
    vector<districts*> path;
    int weight = 0;

    for (const auto& node : districtMap) {
        distance[node.second] = INT_MAX;
        parent[node.second] = nullptr;
    }

    distance[start] = 0;
    priority_queue<pair<int, districts*>, vector<pair<int, districts*>>, greater<pair<int, districts*>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        districts* current = pq.top().second;
        int dist = pq.top().first;
        pq.pop();

        if (dist > distance[current]) {
            continue;
        }

        for (const auto& neighbor : districtGraph[current]) {
            districts* next = neighbor.first;
            int nextDist = neighbor.second;

            if (dist + nextDist < distance[next]) {
                distance[next] = dist + nextDist;
                parent[next] = current;
                pq.push({distance[next], next});
            }
        }
    }

    districts* current = end;
    while (current != nullptr) {
        path.push_back(current);
        districts* prev = parent[current];
        if (prev != nullptr) {
            weight += districtGraph[prev][current];
        }
        current = prev;
    }

    reverse(path.begin(), path.end());

    return {path, weight};
}

void showShortestPath(districts* startDistrict, districts* endDistrict){
    pair<vector<districts*>, int> result = shortestPath(startDistrict, endDistrict);
    clearScreen();
    mainText("Shipping Cost");
    cout << "Shipping path from " << startDistrict->name << " to " << endDistrict->name << " is: \n";
    for (districts* district : result.first) {
        cout << district->name << " -> ";
    }
    cout << endl;
    cout << "Total Cost will be: " << result.second << endl;

    char back;
    cout << "Press b to go back: ";
    cin >> back;
}

void displayAllMarkets(const unordered_map<string, vector<paikariMarket*>>& productMarketMap) {
    mainText("Product Markets");
    cout << "Enter a product name: ";
    string searchProduct;
    cin >> searchProduct;
    clearScreen();

    queue<string> productQueue;
    unordered_set<string> visitedProducts;

    productQueue.push(searchProduct);
    visitedProducts.insert(searchProduct);

    int i = 1;
    int option;

    clearScreen();
    cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t Product Markets\n";
        cout << "-------------------------------------\n";

    while (!productQueue.empty()) {
        string currentProduct = productQueue.front();
        productQueue.pop();

        if (productMarketMap.find(currentProduct) != productMarketMap.end()) {
            cout << "Markets for " << currentProduct << ":" << endl;
            for (paikariMarket* market : productMarketMap.at(currentProduct)) {
                cout << i++ << " Market: " << market->getMarketName() << "\nMarket Address: " << market->getAddress() << endl;
            }
            cout << "Press Market No to see the shipping cost in your area & 0 to go back: ";
            cin >> option;
            if (option == 0) {
                clearScreen();
            } else {
                i = 1;
                for (paikariMarket* market : productMarketMap.at(currentProduct)) {
                    if (i == option) {
                        
                        showShortestPath(districtMap[market->getAddress()], districtMap[loggedInUserAddress]);
                      
                    }
                    ++i;
                }
            }
        }
    }

    if (visitedProducts.find(searchProduct) == visitedProducts.end()) {
        cout << "Product not found!" << endl;
    }

    clearScreen();
}

void showDistributeCost(districts* start) {
    unordered_map<districts*, bool> visited;
    unordered_map<districts*, districts*> parent;
    unordered_map<districts*, int> key;
    int totalWeight = 0;

    for (const auto& node : districtMap) {
        visited[node.second] = false;
        parent[node.second] = nullptr;
        key[node.second] = INT_MAX;
    }

    priority_queue<pair<int, districts*>, vector<pair<int, districts*>>, greater<pair<int, districts*>>> pq;
    pq.push({0, start});
    key[start] = 0;

    while (!pq.empty()) {
        districts* current = pq.top().second;
        pq.pop();

        visited[current] = true;

        for (const auto& neighbor : districtGraph[current]) {
            districts* next = neighbor.first;
            int weight = neighbor.second;

            if (!visited[next] && weight < key[next]) {
                key[next] = weight;
                parent[next] = current;
                pq.push({key[next], next});
            }
        }
    }
    clearScreen();
    mainText("Supply Whole Bangladesh");
    cout << "Minimum cost to supply whole bangladesh from  " << start->getName() << ": \n";
    for (const auto& node : districtMap) {
        if (parent[node.second] != nullptr) {
            districts* current = node.second;
            districts* par = parent[current];
            int weight = key[current];
            totalWeight += weight;
            cout << par->name << " -> " << current->name << " Cost: " << weight << "\n";
        }
    }
    cout << "Total Cost: " << totalWeight << "\n";

    char op;
    cout << "Press b to go back: ";
    cin >> op;
    clearScreen();
}

vector<string> getProductSuggestion(const unordered_map<string, products*>& productMap, int budget) {
    vector<string> suggestedProducts;

    for (const auto& product : productMap) {
        if (product.second->getRetailPrice() <= budget) {
            suggestedProducts.push_back(product.first);
        }
    }

    return suggestedProducts;
}



void suggestProduct(const unordered_map<string, products*>& productMap){
    int budget;
    mainText("Product Suggestion");
    cout << "Enter your budget: $";
    cin >> budget;

    vector<string> suggestedProducts = getProductSuggestion(productMap, budget);

    cout << "Suggested Products within budget $" << budget << ":" << endl;
    unordered_map<string, int> productQuantity;
    
    for (const auto& product : suggestedProducts) {
        cout << product << endl;
    }

    

    cout << "Press 'B' to go back...";
    char back;
    cin >> back;
    clearScreen();
}

void generateCombinations(const unordered_map<string, products*>& productMap, vector<pair<string, int>>& combination, int budget, int currentBudget, int index) {
    if (currentBudget > budget) {
        return;
    }

    if (currentBudget == budget) {
        cout << "Combination: ";
        for (const auto& item : combination) {
            cout << item.first << "(" << item.second << "x) ";
        }
        cout << endl;
        return;
    }

    vector<string> products;
    for (const auto& product : productMap) {
        products.push_back(product.first);
    }

    for (int j = index; j < products.size(); ++j) {
        int productPrice = productMap.at(products[j])->getRetailPrice();
        int maxQuantity = (budget - currentBudget) / productPrice;

        for (int quantity = 1; quantity <= maxQuantity; ++quantity) {
            combination.push_back({products[j], quantity});
            generateCombinations(productMap, combination, budget, currentBudget + (productPrice * quantity), j + 1);
            combination.pop_back();
        }
    }
}



void suggestProductCombinations(const unordered_map<string, products*>& productMap){
    int budget;
    mainText("Product Suggestion");
    cout << "Enter your budget: ";
    cin >> budget;

    clearScreen();
    mainText("Product Suggestion");
    vector<pair<string, int>> combination;
    generateCombinations(productMap, combination, budget, 0, 0);

    cout << "Press '1' to do again & '0' go back: ";
    char option;
    cin >> option;
    if(option=='1'){
        clearScreen();
        suggestProductCombinations(productMap);
    }
}



void calculateShippingCost(){
    cout << "To your address? (y/n): ";
    char opt;
    cin >> opt;
    string source, destination;
    if(opt=='y'){
        cout << "Enter Destination: ";
        cin >> destination;
        showShortestPath(districtMap[loggedInUserAddress], districtMap[destination]);
    }else{
        cout << "Enter Source: ";
        cin >> source;
        cout << "Enter Destination: ";
        cin >> destination;
        showShortestPath(districtMap[source], districtMap[destination]);
    }

}

void userOptions(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketMap) {
    while (true) {
        clearScreen();
        string a ="Welcome " + loggedInUser;
        mainText(a);
        cout << "Choose an option:\n1. Display all products\n2. Display all markets\n3. Suggest products\n4. Suggest Product Combinations\n5. Minimum cost to supply whole Bangladesh\n6. Shipping cost to my address\n9. Log Out\n0. Back\nEnter your choice: ";
        char userOption;
        int budget;
        string place;
        cin >> userOption;
        clearScreen();

        switch(userOption) {
            case '0':
                userOptions(productMap, productMarketMap);
                break;
            case '1':
                displayAllProducts(productMap);
                break;
            case '2':
                displayAllMarkets(productMarketMap);
                break;
            case '3':
                suggestProduct(productMap);
                break;
            case '4':
                suggestProductCombinations(productMap);
                break;
            case '5':
                clearScreen();
                cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t Supply Whole Bangladesh\n";
        cout << "-------------------------------------\n";
                cout << "From which you want to see the cost: ";
                cin >> place;
                clearScreen();
                showDistributeCost(districtMap[place]);
                break;
            case '9':
                isLoggedIn = false; // Logout the user
                cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t We will miss you\n";
        cout << "-------------------------------------\n";
                cout << "Logged out successfully." << endl;
                _sleep(1000);
                initialOptions(productMap, productMarketMap);
                break;
            case '6':
                clearScreen();
                cout << "-------------------------------------\n";
                cout << "\tUddoktader Prothom Thikana\n";
                cout << "-------------------------------------\n";
                cout << "\t Shipping Cost \n";
                cout << "-------------------------------------\n";
                calculateShippingCost();
            default:
                cout << "Invalid option. Please choose again." << endl;
                _sleep(1000);
                userOptions(productMap, productMarketMap);
                clearScreen();
                break;
        }
    }
}


void adminLogin(unordered_map<string, products*>&productMap, unordered_map<string, vector<paikariMarket*>>& productMarketMap){
    string username, password;
    clearScreen();
    mainText("Admin Login");
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;

    if(username=="admin" and password == "admin"){
        adminOptions(productMap, productMarketMap);
    }else{
        cout << "You are not eligible to enter the admin room!";
        _sleep(1000);
        adminLogin(productMap, productMarketMap);
    }
}

void adminOptions(unordered_map<string, products*>& productMap, unordered_map<string,  vector<paikariMarket*>>& productMarketMap) {
    clearScreen();
    while (true) {
        mainText("Admin Options");
        cout << "Choose an option:\n1. Add a Product\n2. Add a Market\n3. Display all Products\n4. Display all Markets\n5. Display All Places\n6. Cost to supply whole Bangladesh\n7. Add a route\n0. Back\nEnter your choice: ";
        char adminOption;
        string place;
        string source, destination;
        int weight;
        cin >> adminOption;
        clearScreen();

        switch (adminOption) {
            case '0':
                return;
            case '1': {
                string productName, marketName, marketAddress;
                int retailPrice, sellingPrice;

                cout << "Enter Product Name: ";
                cin >> productName;
                cout << "Enter Retail Price: ";
                cin >> retailPrice;
                cout << "Enter Selling Price: ";
                cin >> sellingPrice;
                cout << "Enter " << productName << "'s market name: ";
                cin >> marketName;
                cout << "Enter " << productName << "'s market address: ";
                cin >> marketAddress;

                products* newProduct = new products(productName, retailPrice, sellingPrice);
                productMap[productName] = newProduct;
                paikariMarket* newMarket = new paikariMarket(productName, retailPrice, sellingPrice, marketName, marketAddress);
                addConnection(newProduct, newMarket);
                productMarketMap[productName].push_back(newMarket);
                cout << "Product added successfully!" << endl;
                clearScreen();
                break;
            }
            case '2': {
                string productName, marketName, marketAddress;
                cout << "Enter Product Name: ";
                cin >> productName;

                if (productMap.find(productName) != productMap.end()) {
                    cout << "Enter Market Name: ";
                    cin >> marketName;
                    cout << "Enter Market Address: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, marketAddress);

                    paikariMarket* newMarket = new paikariMarket(productName, 0, 0, marketName, marketAddress);
                    productMarketMap[productName].push_back(newMarket);
                    cout << "Market added successfully!" << endl;
                    _sleep(1000);
                    clearScreen();
                } else {
                    cout << "Product not found!" << endl;
                    _sleep(1000);
                    adminOptions(productMap, productMarketMap);
                    clearScreen();
                }
                break;
            }
            case '3':
            clearScreen();
                cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t Products\n";
        cout << "-------------------------------------\n";
                displayAllProducts(productMap);
                break;
            case '4':
            clearScreen();
            cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t Markets\n";
        cout << "-------------------------------------\n";
                displayAllMarkets(productMarketMap);
                break;
            case '5':
            clearScreen();
            cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t Places Available\n";
        cout << "-------------------------------------\n";
                displayAllEdgesWithWeights();
                break;
            case '6':
                clearScreen();
                cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t Supply Whole Bangladesh\n";
        cout << "-------------------------------------\n";
                cout << "From which you want to see the cost: ";
                cin >> place;
                clearScreen();
                showDistributeCost(districtMap[place]);
                break;
            case '7':
                clearScreen();
                cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t Supply Whole Bangladesh\n";
        cout << "-------------------------------------\n";
                cout << "Enter the Source: ";
                cin >> source;
                cout << "Enter the destination: ";
                cin >> destination;
                cout << "Enter distance: ";
                cin >> weight;

                addEdge(districtMap[source], districtMap[destination], weight);
                cout << "Root added Successfully";
                _sleep(1000);
                break;
            default:
                cout << "Invalid option. Please choose again." << endl;
                _sleep(1000);
                adminOptions(productMap, productMarketMap);
                clearScreen();
                break;
        }
    }
}

int main();

void initialOptions(const unordered_map<string, products*>& productMap, const unordered_map<string, vector<paikariMarket*>>& productMarketMap) {
    clearScreen();
    mainText("Decide Your Future");
    cout << "Choose an option:" << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";

    char option;
    cin >> option;
    clearScreen();

    switch (option) {
        case '0':
            main();
            break;
        case '1':
            registerUser(productMap, productMarketMap);
            break;
        case '2':
            loginUser(productMap, productMarketMap);
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            _sleep(1000);
            initialOptions(productMap, productMarketMap);
    }
}

int main() {
    unordered_map<string, products*> productMap;
    unordered_map<string, vector<paikariMarket*>> productMarketMap;

    initializeDistricts();

    products* shoes = new products("Shoes", 70, 120);
products* dress = new products("Dress", 80, 150);
products* watch = new products("Watch", 100, 200);
products* laptop = new products("Laptop", 500, 1000);
products* smartphone = new products("Smartphone", 300, 600);
products* hat = new products("Hat", 40, 80);
products* socks = new products("Socks", 20, 30);
products* jacket = new products("Jacket", 120, 180);
products* glasses = new products("Glasses", 50, 90);
products* scarf = new products("Scarf", 25, 40);

    productMap["Shoes"] = shoes;
productMap["Dress"] = dress;
productMap["Watch"] = watch;
productMap["Laptop"] = laptop;
productMap["Smartphone"] = smartphone;
productMap["Hat"] = hat;
productMap["Socks"] = socks;
productMap["Jacket"] = jacket;
productMap["Glasses"] = glasses;
productMap["Scarf"] = scarf;

    paikariMarket* dhakaMarket = new paikariMarket("Shirt", 50, 60, "Dhaka Market", "Dhaka");
paikariMarket* sylhetMarket = new paikariMarket("Pant", 60, 80, "Sylhet Market", "Sylhet");
paikariMarket* chittagongMarket = new paikariMarket("Hat", 30, 100, "Chittagong Market", "Chittagong");
paikariMarket* rajshahiMarket = new paikariMarket("Bag", 40, 70, "Rajshahi Market", "Rajshahi");
paikariMarket* khulnaMarket = new paikariMarket("Market 5", 60, 90, "Khulna Market", "Khulna");
paikariMarket* barishalMarket = new paikariMarket("Market 6", 40, 70, "Barishal Market", "Barishal");
paikariMarket* rangpurMarket = new paikariMarket("Market 7", 30, 80, "Rangpur Market", "Rangpur");
paikariMarket* mymensinghMarket = new paikariMarket("Market 8", 50, 60, "Mymensingh Market", "Mymensingh");
paikariMarket* coxMarket = new paikariMarket("Market 9", 70, 100, "Cox's Bazar Market", "Cox's Bazar");
paikariMarket* boguraMarket = new paikariMarket("Market 10", 45, 75, "Bogura Market", "Bogura");

addConnection(shoes, dhakaMarket);
addConnection(dress, sylhetMarket);
addConnection(watch, chittagongMarket);
addConnection(laptop, rajshahiMarket);
addConnection(smartphone, khulnaMarket);
addConnection(hat, barishalMarket);
addConnection(socks, rangpurMarket);
addConnection(jacket, mymensinghMarket);
addConnection(glasses, coxMarket);
addConnection(scarf, boguraMarket);

productMarketMap["Shoes"].push_back(dhakaMarket);
productMarketMap["Dress"].push_back(sylhetMarket);
productMarketMap["Watch"].push_back(chittagongMarket);
productMarketMap["Laptop"].push_back(rajshahiMarket);
productMarketMap["Smartphone"].push_back(khulnaMarket);
productMarketMap["Hat"].push_back(barishalMarket);
productMarketMap["Socks"].push_back(rangpurMarket);
productMarketMap["Jacket"].push_back(mymensinghMarket);
productMarketMap["Glasses"].push_back(coxMarket);
productMarketMap["Scarf"].push_back(boguraMarket);

    char option;

    while(true){
        clearScreen();
        cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t  Choose Your Role:" << endl;
        cout << "\t  1. User" << endl;
        cout << "\t  2. Admin" << endl;
        cout << "\t  0. Exit" << endl;
        cout << "\t  Enter your choice: ";

        cin >> option;

        if(option=='1'){
            if(isLoggedIn){
                userOptions(productMap, productMarketMap);
            }else{
                initialOptions(productMap, productMarketMap);
            }
        }else if(option=='2'){
            adminLogin(productMap, productMarketMap);
        }else if(option=='0'){
            cout << "-------------------------------------\n";
        cout << "\tUddoktader Prothom Thikana\n";
        cout << "-------------------------------------\n";
        cout << "\t Good Bye\n";
        cout << "-------------------------------------\n";
            cout << "Exiting..." << endl;
            break;
        }else{
            cout << "Wrong Choice! Try Again" << endl;
            cin >> option;
        }
    }

    

    // Clean up memory (deallocate objects)
    for (const auto& pair : productMap) {
        delete pair.second;
    }

    for (const auto& pair : productMarketMap) {
        for (paikariMarket* market : pair.second) {
            delete market;
        }
    }

    return 0;
}
