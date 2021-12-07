#include<bits/stdc++.h>

using namespace std;

#define For(i, a, b) for(int i = a; i <= b; ++ i)
#define pb push_back
#define index first
#define value second
#define Ni pair<Node*, int>

const int N = 1e5 + 5;
const int num_att = 4, numFold = 5;
int min_att[4] = {1, 1, 1, 1};
int max_att[4] = {5, 5, 5, 5};

struct Node{
    int index;
    int value;
    char label;
    Node* left;
    Node* right;
    Node(){
        left = NULL;
        right = NULL;
        index = -1;
        value = -1;
        label = '0';
    }
};

struct Data{
    char label;
    int att[num_att];
};

double goodnessOfBestSplit(vector<Data> data, vector<Data> data_left, vector<Data> data_right){
    double ratioLeft = (double) data_left.size() / data.size();
    double ratioRight = 1 - ratioLeft;

    map<char, int> ma_left;
    for(Data x : data) ma_left[x.label] ++;

    map<char, int> ma_right;
    for(Data x : data) ma_right[x.label] ++;

    double res = 0;
    for(auto it : ma_left){
        double p_left = (double) it.second / data_left.size();
        double p_right = ma_right[it.first] / data_right.size();
        res += abs(p_left - p_right);
    }

    for(auto it : ma_right){
        if(ma_left[it.first] == 0){
            double p_right = (double) it.second / data_right.size();
            res += p_right;
        }
    }

    return 2 * ratioLeft * ratioRight * res;
}

double getGiniEachGroup(vector<Data> data){
    map<char, int> ma;
    for(Data x : data) ma[x.label] ++;

    double gini = 1;
    for(auto it : ma){
        double p = (double) it.second / data.size();
        gini -= p * p;
    }
    return gini;
}

double getGiniSplit(vector<Data> data, vector<Data> data_left, vector<Data> data_right){
    double ratioLeft = (double) data_left.size() / data.size();
    double ratioRight = 1 - ratioLeft;
    return getGiniEachGroup(data_left)* ratioLeft + getGiniEachGroup(data_right)* ratioRight;
}

double getEntropyEachGroup(vector<Data> data){
    map<char, int> ma;
    for(Data x : data) ma[x.label] ++;

    double entropy = 0;
    for(auto it : ma){
        double p = (double) it.second / data.size();
        entropy += p * log(p) / log(2);
    }
    return -entropy;
}

double getGainSplit(vector<Data> data, vector<Data> data_left, vector<Data> data_right){
    double ratioLeft = (double) data_left.size() / data.size();
    double ratioRight = 1 - ratioLeft;
    return getEntropyEachGroup(data)
           - getEntropyEachGroup(data_left)* ratioLeft
           - getEntropyEachGroup(data_right)* ratioRight;
}

double getGainRatio(vector<Data> data, vector<Data> data_left, vector<Data> data_right){
    double ratioLeft = (double) data_left.size() / data.size();
    double ratioRight = 1 - ratioLeft;

    double splitInfo = -(ratioLeft * log(ratioLeft) + ratioRight * log(ratioRight)) / log(2);
    return getGainSplit(data, data_left, data_right) / splitInfo;
}

void splitData(vector<Data> data, vector<Data> &data_left, vector<Data> &data_right, int index, int value){
    data_left.clear(); data_right.clear();
    for(Data x : data){
        if(x.att[index] <= value) data_left.pb(x);
        else data_right.pb(x);
    }
}

char findLabel(vector<Data> &data){
    map<char, int> ma;
    for(Data x : data) ma[x.label] ++;

    int Max = 0; char res;
    for(auto it : ma){
        if(it.second > Max){
            Max = it.second;
            res = it.first;
        }
    }
    return res;
}

void buildTree(Node *&pa, vector<Data> data, int depth, int maxDepth, int minSize){
    pa = new Node();
    if(depth >= maxDepth || data.size() <= minSize || getGiniEachGroup(data) == 0){
        pa -> label = findLabel(data);
        return;
    }

    vector<Data> data_left, data_right;
    double maxValue = -10;

    For(i, 0, num_att - 1)
    For(j, min_att[i], max_att[i]){
        splitData(data, data_left, data_right, i, j);
        if(data_left.empty() || data_right.empty()) continue;

        double giniSplit = -getGiniSplit(data, data_left, data_right);       // get max
        double gainSplit = getGainSplit(data, data_left, data_right);        // get max
        double gainRatio = getGainRatio(data, data_left, data_right);        // get max

        if(maxValue < gainSplit){
            maxValue = gainSplit;
            pa -> index = i;
            pa -> value = j;
        }
    }

    splitData(data, data_left, data_right, pa -> index, pa -> value);
    buildTree(pa -> left, data_left, depth + 1, maxDepth, minSize);
    buildTree(pa -> right, data_right, depth + 1, maxDepth, minSize);
}

char checkData(Node *&pa, Data current_data){
    if(pa -> left == NULL) return pa -> label;
    if(current_data.att[pa -> index] < pa -> value) return checkData(pa -> left, current_data);
    return checkData(pa -> right, current_data);
}

double calculateAccuracy(vector<Data> data, Node *&root){
    int cnt  = 0;
    for(Data current_data : data) cnt += (current_data.label == checkData(root, current_data));
    return (double) cnt / data.size();
}

void showTree(Node *&pa, int depth){
    if(pa -> left == NULL){
        cout << pa -> label << ' ' << depth << endl;
        return;
    }
    cout << pa -> index << ' ' << pa -> value << ' ' << depth << endl;

   showTree(pa -> left, depth + 1);
   showTree(pa -> right, depth + 1);
}

void showWrongData(vector<Data> data, Node *&root){
    cout << "data is guessed wrongly:" << endl;
    for(Data current_data : data)
        if(checkData(root, current_data) != current_data.label){
            cout << current_data.label << ' ';
            For(i, 0, num_att - 1) cout << current_data.att[i] << ' ';
            cout << endl;
        }
    cout << endl;
}

void showConfusionMatrix(vector<Data> data, Node *&root){
    int id[26];
    id['L' - 'A'] = 0; id['B' - 'A'] = 1; id['R' - 'A'] = 2;
    int cnt[3][3];
    For(i, 0, 2) For(j, 0, 2) cnt[i][j] = 0;

    for(Data current_data : data){
        char predict_label = checkData(root, current_data);
        cnt[id[current_data.label - 'A']][id[predict_label - 'A']] ++;
    }

    cout << "Total: " << data.size() << "     Predict as: L"  << "     Predict as: B" << "     Predict as: R\n";
    for(int i = 0; i < 3; ++ i){
        if(i == 0) cout << "True: L           ";
        else if(i == 1) cout << "True: B           ";
        else cout << "True: R           ";

        for(int j = 0; j < 3; ++ j){
            cout << cnt[i][j] << "                  ";
        }
        cout << endl;
    }
}

void inputData(vector<Data> &data, string inputfile){
    ifstream fileInput(inputfile);
    while(!fileInput.eof()){
        string input;
        fileInput >> input;

        Data x;
        x.label = input[0];
        For(i, 0, 4) x.att[i] = input[(i + 1) << 1] - '0';
        data.pb(x);
    }
    fileInput.close();
}

void shuffleData(vector<Data> &data){
    random_shuffle(data.begin(), data.end());
}

void crossValidation(vector<Data> data, vector<Data> *fold){
    // 202 L, R - 35 B
    map<char, int> ma;
    ma['L'] = 0;
    ma['B'] = 1;
    ma['R'] = 2;

    vector<Data> dataLabel[3];

    for(Data x : data) dataLabel[ma[x.label]].pb(x);

    For(i, 0, 2)
    for(int j = 0; j < dataLabel[i].size(); ++ j)
        fold[j % numFold].pb(dataLabel[i][j]);
}

void findDataTrainDataTest(vector<Data> &data_train, vector<Data> &data_valid, vector<Data> *fold, int idFold){
    data_train.clear();
    data_valid.clear();
    For(i, 0, numFold - 1){
        if(i == idFold) data_valid = fold[i];
        else for(Data x : fold[i]) data_train.pb(x);
    }
}

signed main(){
    srand(time(NULL));
    vector<Data> data_train, data_valid, data_hidden;
    vector<Data> fold[numFold];

    inputData(data_train, "train.txt");
    inputData(data_valid, "valid.txt");

    crossValidation(data_train, fold);

    Node *root = NULL;
    double bestSolution = 0;
    int maxDepth, minSize;
    For(i, 3, 10) For(j, 3, 10){

        double sumKTrain = 0;
        For(idFoldValid, 0, numFold - 1){
            vector<Data> newData_train, newData_valid;
            findDataTrainDataTest(newData_train, newData_valid, fold, idFoldValid);
            buildTree(root, newData_train, 0, i, j);
            sumKTrain += calculateAccuracy(newData_valid, root);
        }
        if(bestSolution < sumKTrain){
            bestSolution = sumKTrain;
            maxDepth = i;
            minSize = j;
        }
    }

    buildTree(root, data_train, 0, maxDepth, minSize);

    cout << "maxdepth: " << maxDepth << endl;
    cout << "minsize: " << minSize << endl;

    cout << "accuracy of valid data: " << calculateAccuracy(data_valid, root) << endl;
    cout << "accuracy of train data: " << calculateAccuracy(data_train, root) << endl;

    cout << endl;
    cout << "show tree in preorder way: " << endl;
    cout << "form (attribute, value of attribute to compare, depth): " << endl;
    showTree(root, 0);
    cout << endl;

    showWrongData(data_valid, root);
    showConfusionMatrix(data_valid, root);
}
