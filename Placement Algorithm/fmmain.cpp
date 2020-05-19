// header files
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <limits>
#include "cell.h"
using namespace std;

vector<int> heapVector;			                         //this vector will be used as help in order to store the gain
map<int, vector<string> > storeGainBucket;               //this is hash table data structure which will store all the gains
map<int, vector<string> > nodenetMap;                    // this is a hash map which which store all the net information
vector<string> nodecellId;		                         //this vector will store all the cell ids
map<string, Cell> nodecellData;                          // this map will store object of each cell class
int net;
float ratioFactor = 0.6;                                 
int startcut;											 //it will store the starting cut
int mincut;												 //it will store the final cut
int areaTotal;										     //it will have the sum of the areas of all cells

void storeGain();										 // gain  bucket
void updateGain();                                       // update in each pass
void moveNode(); 
int doCutSize();
void makePartition();
bool seeAreaLimit();
void readhgr(string f);
void readAreaCell(string file);
int calculateGain(string cell);


																
vector<string> dividestring(const string& s, char delimiter) {          // this function is to divide the input into the readable format
	stringstream ss(s);
	string line;
	vector<string> part;
	while (getline(ss, line, delimiter)) {
		part.push_back(line);
	}
	return part;
}
string stringline;

																		//in this function all the area file of the cell will be readed
void readAreaCell(string file) {	
	ifstream arefile(file.c_str());
	if (!arefile) {
		cout << "No file in the location" << endl;
	}
	else { 
		while (getline(arefile, stringline)) {							//here input area file is readed and passed into the map
			vector<string> temp = dividestring(stringline, ' ');
			Cell c;
			c.Id = temp.at(0);
			c.lockStatus = false;
			c.areaOfCell = atoi(temp.at(1).c_str());
			heapVector.push_back(atoi(temp.at(1).c_str()));
			push_heap(heapVector.begin(), heapVector.end());
			nodecellId.push_back(temp.at(0));
			nodecellData.insert(pair<string, Cell>(temp.at(0), c));
		}
	}
}

void readhgr(string f) {                        					//creates a partition  of nodes in the map
	string l1;                                                      //first half goes to partition 0 and second half to partition 1
	string l2;
	ifstream hgrfile(f.c_str());
	if (!hgrfile) {
		cout << "No file in the location" << endl;
	}
	else {                                                          //now we start 1st line of file and start dividing to read the input
		std::getline(hgrfile, l1);
		vector<string> temp = dividestring(l1, ' ');
		net = atoi(temp.at(0).c_str());
		int num = 0;
		while (getline(hgrfile, l2)) {
			vector<string> arr = dividestring(l2, ' ');
			if (arr.size() > 0) {
				int k = 0;
				while (k < arr.size()) {                             //we add all the netid to nodecelldata vector, in order to find how many 
					nodecellData[arr.at(k)].netList.push_back(num);  //cells are they in that net
					k++;
				}				                                                              
				nodenetMap.insert(pair<int, vector<string> >(num, arr)); //insert netid to the map
				num++;
			}
		}
	}
}
void makePartition() {                                             //in this function we divide the cell into two partition based on area  
	int length = nodecellData.size();                              
	areaTotal = 0;
	for (int i = 0; i < nodecellId.size(); i++){
		string id = nodecellId.at(i).c_str();
		areaTotal += nodecellData[id].areaOfCell;
	}
	long areaT = 0;
	long finalArea = (ratioFactor) * (areaTotal);
	int index = 0;
	int i = 0;
	while (i < length) {
		string id = nodecellId.at(i).c_str();
		areaT += nodecellData[id].areaOfCell;

		if (areaT >= finalArea) {
			index = i - 1;
			break;
		}
			i++;
	}
	int j = 0;
	while (j < length) {
		if (j <= index) {
			nodecellData[nodecellId.at(j)].partition = 0;
		}
		else {
			nodecellData[nodecellId.at(j)].partition = 1;
		}
		j++;
	}
}
int calculateGain(string cell) {											//in this we calculate the gain using FS - TE
	int fs = 0;
	int te = 0;
	vector<int> netList = nodecellData[cell].netList;                       //take all the netId from nodecell data
	for (int i = 0; netList.size(); i++) {
		vector<string> netvec = nodenetMap[netList.at(i)];                  // then take all the cell using cellId
		int ns1 = 0;
		int ns2 = 0;
		for (int j = 0; j < netvec.size(); j++) {
			string node = netvec.at(j).c_str();
			if (cell != node) {                    
				if (nodecellData[cell].partition == nodecellData[node].partition) {
					ns1++;
				}
				if (nodecellData[cell].partition != nodecellData[node].partition) {
					ns2++;
				}
			}
		}
		if (ns1 == 0) {
			fs++;
		}
		if (ns2 == 0) {
			te++;
		}

	}
	return fs - te;
}
bool seeAreaLimit() {
	int area = 0;
	int j = 0;
	while (j < nodecellId.size()) {
		string id = nodecellId.at(j).c_str();
		if (nodecellData[id].partition == 0) {
			area += nodecellData[id].areaOfCell;
		}
		j++;
	}	
	int cellMaxArea = heapVector.front();
	if (((ratioFactor * areaTotal - cellMaxArea) <= area) && (area <= (ratioFactor * areaTotal + cellMaxArea))) {
		return true;
	}
	else {
		return false;
	}
}
int doCutSize() {
	int cutsize = 0;
	int i = 0;
	while (i < nodenetMap.size()) {
		string startNode = nodenetMap[i].at(0);
		int j = 0;
		while (j < nodenetMap[i].size()) {
			if (nodecellData[startNode].partition != nodecellData[nodenetMap[i].at(j)].partition) {
				cutsize++;
				break;
			}
			j++;
		}
		i++;
	}
	return cutsize;
}
void storeGain() {											        //this is a bucket which contain gain
	storeGainBucket.clear();										//start from empty
	int i = 0;
	while (i < nodecellId.size()) {
		string cellId = nodecellId.at(i).c_str();
		if (nodecellData[cellId].checkIsLocked() == false) {       //do for cell which are not locked
			int gainVal = calculateGain(cellId);
			nodecellData[cellId].areaOfCell = gainVal;
			map<int, vector<string>> ::iterator itr;
			itr = storeGainBucket.find(gainVal);		                                     
			if (itr == storeGainBucket.end()) {                   //if gain is not found, we create a vector and add cell 
				vector<string> temparr;
				temparr.push_back(cellId);
				storeGainBucket.insert(pair<int, vector<string>>(gainVal, temparr));
			}
			else {
				itr->second.push_back(cellId);
			}
		}
		i++;
	}
}
void updateGain(string cellno) {
	std::vector<int> arrList = nodecellData[cellno].netList;        //take all the netId from nodecell data
	int i = 0;
	while (i < arrList.size()) {									//we iterate on every cell corresponding to the net
		std::vector<string> netarr = nodenetMap[arrList.at(i)];		
		int j = 0;
		while (j < netarr.size()) {
			string cellpos = netarr.at(j);
			if (cellno != cellpos) {
				if (nodecellData[cellpos].checkIsLocked() == false) {
					long nodegain = nodecellData[cellpos].areaOfCell;         //store the gain
					//find the position of the node in the gain bucket and remove it
					// std::vector<string>::iterator pos=gainBucket[ngain].find(node);
					int k = 0;
					while (k < storeGainBucket[nodegain].size()) {
						if (storeGainBucket[nodegain].at(k) == cellpos) {
							storeGainBucket[nodegain].erase(storeGainBucket[nodegain].begin() + k);
							//compute new gain and add it to the new position in the gain bucket
							int updatedgain = calculateGain(cellpos);
							nodecellData[cellpos].areaOfCell = updatedgain;
							storeGainBucket[updatedgain].push_back(cellpos);
							break;
						}
						k++;
					}
				}
			}
			j++;
		}
		i++;
	}
}
//moving cell from one partition to partition
void moveNode() {
	//starting from highest gain
	map<int, vector<string>>::reverse_iterator itr = storeGainBucket.rbegin();
	while (itr != storeGainBucket.rend()) {
		string nodeId = "";
		int mark = 0;
		int i = 0;
		while (i < (itr->second).size()) {
			string node = (itr->second).at(i);
			if (nodecellData[node].checkIsLocked() == false) {
				nodecellData[node].decidePartition();
				if (seeAreaLimit() == false) {
					nodecellData[node].decidePartition();
					(itr->second).erase((itr->second).begin() + i);
					continue;
				}
				else {
					//locking the node
					nodecellData[node].setLockStatus(true);
					nodeId = node;
					int cut = doCutSize();
					if (cut < mincut) {
						mincut = cut;
					} 
					else if (cut > mincut) {
						return;
					}
					storeGainBucket[itr->first].erase(find(storeGainBucket[itr->first].begin(), storeGainBucket[itr->first].end(), node));
					mark = 1;
					break;
				}
			}
			i++;
		}
		if (mark == 1) {
			updateGain(nodeId);
			itr = storeGainBucket.rbegin();
		}
		itr++;
	}
}
int main() {
	make_heap(heapVector.begin(), heapVector.end());
	readAreaCell("ibm01.are");
	makePartition();
	readhgr("ibm01.hgr");
	storeGain();
	startcut = doCutSize();
	mincut = startcut;
	moveNode();
	cout << "starting cut size: " << startcut << endl;
	cout << "minimum cut size: " << mincut << endl;
	cout << "maximum gain: " << startcut - mincut << endl;
	return 0;
}