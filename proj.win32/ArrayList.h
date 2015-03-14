using namespace std;  

class ArrayList {  

public:  
	ArrayList();  
	ArrayList(int initSize);  
	ArrayList(const ArrayList &original);  
	~ArrayList();  

	void insert(int position, int value);   
	int get(int position);    

	int getSize();  
	void resize(int newSize);  

	int &operator[](int index);  
	bool operator==(ArrayList);  
	bool operator!=(ArrayList);  

private:  
	int size;    
	int *array;  

};  

ArrayList::ArrayList() {  
	ArrayList::ArrayList(5);  
}  

ArrayList::ArrayList(int initSize) {  
	size = initSize;  
	array = new int[size];  
	for (int i = 0; i < size; i++) {  
		array[i] = 0; // Fill with zeroes  
	}  
}  

ArrayList::ArrayList(const ArrayList &original) {  
	size  = original.size;  
	array = new int[size];  
	for (int i = 0; i < size; i++) {  
		array[i] = original.array[i];  
	}  
}  

ArrayList::~ArrayList() {  
	delete[] array;  
}  


void ArrayList::insert(int position, int value) {  
	array[position] = value;  
}  

int ArrayList::get(int position) {  
	return array[position];  
}  

int ArrayList::getSize() {  
	return size;  
}  

void ArrayList::resize(int newSize) {  
	int *temp;  
	temp = new int[newSize];  
	for (int i = 0; i < (newSize); i++) {  
		temp[i] = array[i];  
	}  
	delete[] array;  
	array = temp;  
	size = newSize;  
}  

bool ArrayList::operator==(ArrayList a)  {  
	if (a.size != size) return false;  

	for (int i = 0; i < (a.size); i++) {  
		if (a[i] != array[i]) return false;  
	}  
}  

bool ArrayList::operator!=(ArrayList a)  {  
	if (a.size != size) return true;  

	for (int i = 0; i < (a.size); i++) {  
		if (a[i] != array[i]) return true;  
	}  
}  

int &ArrayList::operator[](int index) {  
	if ((index - 1) > size) {  
		resize(index + 1);  
	}  
	return array[index]; // returned as a reference  
}  