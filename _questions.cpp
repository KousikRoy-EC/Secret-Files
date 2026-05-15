#include <bits/stdc++.h>
using namespace std;

// 1. Prime no

/*
bool isPrime(int num){
    if(num<=1){
        return false;
    }

    for (int i = 2; i <= sqrt(num); i++)
    
       if(num%i==0){
        return false;
       }
    }

    return true;
}

int main(){
    int num;
    cout<<"Enter no to check Prime or not";
    cin>>num;
    isPrime(num)?cout<<"Prime":cout<<"Not Prime";
    return 0;
}
*/

// 2. Fibonacci series

/*
void fibonacci(int num){
    int num1=0;
    int num2=1;
    int sum;
    cout<<num1<<" "<<num2<<" ";

    for(int i=0;i<num-2;i++){
        sum=num1+num2;
        cout<<sum<<" ";
        num1=num2;
        num2=sum;
    }
}

int main(){
    int num;
    cout<<"Enter no to print fibonacci series";
    cin>>num;
    fibonacci(num);
    return 0;
}
*/

// 3. Pallindrome no

/*
bool checkPallindrom(int num){
    string converted=to_string(num);
    string rev=converted;
    reverse(rev.begin(),rev.end());
    return rev==converted ? true : false;
}



bool checkPallindrom(int num)
{
    int revNum = 0;
    int temp;
    int orginalNum = num;

    while (num)
    {
        temp = num % 10;
        revNum = revNum * 10 + temp;
        num = num / 10;
    }

    return revNum == orginalNum;
}

int main()
{
    int num;
    cout << "Enter no to check whether it is pallindrome or not ? ";
    cin >> num;
    if (checkPallindrom(num))
    {
        cout << "Pallindrome";
    }
    else
    {
        cout << "Not Pallindrome";
    }
    return 0;
}

*/





// lcm and gcd of numbers

/*
int gcd(int num1,int num2){
    if(num2==0){
        return num1;
    }
    return gcd(num2,num1%num2);

    ||


    while (num2 != 0) {
        int temp = num2;
        num2 = num1 % num2;
        num1 = temp;
    }
    return num1;
}

int lcm(int num1,int num2){
    return (num1*num2)/gcd(num1,num2);
}

int main(){
    int num1,num2;
    cout<<"Enter two numbers";
    cin>>num1>>num2;
    cout<<"The gcd of two numbers is : "<<gcd(num1,num2);
    cout<<"The lcm of two numbers is : "<<lcm(num1,num2);
    return 0;
}
*/





// factorial
/*
int factorial(int num){
    if(num==1){
        return 1;
    }
    return num*factorial(num-1);
}

int main(){
    int num;
    cout<<"Enter no to find Factorial";
    cin>>num;
    cout<<"The factorial of no is : "<<factorial(num);
    return 0;
}
*/

// 4. Armstrong no

/*
unsigned int cube(unsigned int num)
{
    return num * num * num;
}

bool Armstrong(int num)
{
    int originalNum = num;
    int sum=0;
    int temp;
    while (num)
    {
        temp = num % 10;
        sum += cube(temp);
        num = num / 10;
    }
cout<<sum;
    return sum == originalNum ? true : false;
}

int main()
{
    int num;
    cout << "Enter no to find Armstrong or not";
    cin >> num;
    cout <<"\n The no is"<<Armstrong(num);
    return 0;
}

*/

// decimal to binary

/*
void decimalToBinary(int decimal) {
    if (decimal == 0) {
        cout << "0";
        return;
    }

    int binary[32];
    int i = 0;

    while (decimal) {
        binary[i] = decimal % 2;
        decimal /= 2;
        i++;
    }

    for (int j = i - 1; j >= 0; j--) {
        cout << binary[j];
    }
}

int main() {
    int decimal;
    cout << "Enter a decimal number: ";
    cin >> decimal;

    cout << "Binary representation: ";
    decimalToBinary(decimal);

    return 0;
}
*/

// A year is a leap year if the following conditions are satisfied:

// The year is multiple of 400.
// The year is multiple of 4 and not multiple of 100.

// Anagram
/*
bool anagram(string st1,string st2){
    if(st1.length()!=st2.length()){
        return false;
    }

    char count[256]={0};

    for(int i=0;i<st1.length();i++){
        count[st1[i]]++;
        count[st2[i]]--;
    }

    for(int i=0;i<256;i++){
        if(count[i]!=0){
            return false;
        }
    }
    return true;
}


int main(){
    string st1="listen";
    string st2="silent";

    if(anagram(st1,st2)){
        cout<<"Anagram";
    }
    else{
        cout<<"Not Anagram";
    }
return 0;
}
*/

// program for malloc and calloc in c++
/*
int main(){
    int *ptr;
    int n;
    cout<<"Enter no of elements";
    cin>>n;
    ptr=(int*)malloc(n*sizeof(int));
    if(ptr==NULL){
        cout<<"Memory not allocated";
    }
    else{
        cout<<"Memory allocated successfully";
        for(int i=0;i<n;i++){
            ptr[i]=i+1;
        }
        cout<<"The elements of array are : ";
        for(int i=0;i<n;i++){
            cout<<ptr[i]<<" ";
        }
    }
    return 0;
}

*/

// program for calloc
/*
int main(){
    int *ptr;
    int n;
    cout<<"Enter no of elements";
    cin>>n;
    ptr=(int*)calloc(n,sizeof(int));
    if(ptr==NULL){
        cout<<"Memory not allocated";
    }
    else{
        cout<<"Memory allocated successfully";
        for(int i=0;i<n;i++){
            ptr[i]=i+1;
        }
        cout<<"The elements of array are : ";
        for(int i=0;i<n;i++){
            cout<<ptr[i]<<" ";
        }
    }
    return 0;
}
*/

// structure pointer

/*
struct Person {
    string name;
    int age;
};

int main() {

    Person person;
    Person* personPtr;
    personPtr = &person;
    personPtr->name = "John Doe";
    personPtr->age = 25;

    cout << "Name: " << personPtr->name << endl;
    cout << "Age: " << personPtr->age << endl;

    return 0;
}
*/

// Declaring A Pointer To A Function

/*
int addition(int num1,int num2){
    return num1+num2;
}

int main(){
    int (*ptr)(int,int)=addition; // Declaring A Pointer To A Function
    cout<<(*ptr)(2,3);  // Calling A Function Through Function Pointer
    return -1;
}
*/

// Passing a Function's Address as an Argument to Other Function

/*
int addition(int num1,int num2){
    return num1+num2;
}

void print(int (*ptr)(int,int)){
    cout<<(*ptr)(2,3);
}

int main(){
    print(addition);
    return -1;
}
*/

// functional array pointer

/*
int addition(int num1,int num2){
    return num1+num2;
}

int subtraction(int num1,int num2){
    return num1-num2;
}

int multiplication(int num1,int num2){
    return num1*num2;
}

int division(int num1,int num2){
    return num1/num2;
}

int main(){
    int (*ptr[4])(int,int)={addition,subtraction,multiplication,division};
    cout<<ptr[0](2,3)<<endl;
    cout<<ptr[1](2,3)<<endl;
    cout<<ptr[2](2,3)<<endl;
    cout<<ptr[3](2,3)<<endl;
    return -1;
}
*/

/*

union Data
{
    int intValue;
    float floatValue;
    char charValue;
};

int main()
{
    Data myData ;

    myData.intValue = 42;
    std::cout << "Integer value: " << myData.intValue << std::endl;

    myData.floatValue = 3.14;
    std::cout << "Float value: " << myData.floatValue << std::endl;
    std::cout << "Integer value: " << myData.intValue << std::endl; // Accessing intValue after modifying floatValue

    myData.charValue = 'A';
    std::cout << "Character value: " << myData.charValue << std::endl;
    std::cout << "Float value: " << myData.floatValue << std::endl; // Accessing floatValue after modifying charValue

    return 0;
}

*/

// pattern 1

/*

*
* *
* * *
* * * *
* * * * *



int main(){
    char arr[5][5];

    for(int i=0;i<5;i++){
        for(int j=0;j<=i;j++){
            arr[i][j]='*';
        }
    }

    cout<<"Printing pattern \n";

    for(int i=0;i<5;i++){
        for(int j=0;j<=i;j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }


    return -1;
}

*/

// Pattern 2

/*

* * * * *
  * * * *
    * * *
      * *
        *




int main(){
    char arr[5][5];

    for(int i=0;i<5;i++){
        for (int k = 0;k<i; k++)
        {
            arr[i][k]=' ';
        }

        for(int j=i;j<5;j++){
            arr[i][j]='*';
        }
    }

    cout<<"Printing pattern \n";

    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }


    return -1;
}


*/

// pattern 3

/*
* * * * *
* * * *
* * *
* *
*





int main(){
    char arr[5][5];

    for(int i=0;i<5;i++){
        for(int j=0;j<5-i;j++){
            arr[i][j]='*';
        }
    }

    cout<<"Printing pattern \n";

      for(int i=0;i<5;i++){
        for(int j=0;j<5-i;j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
    return -1;
}

*/

// pattern 4

/*
1
1 2
1 2 3
1 2 3 4
1 2 3 4 5

int main(){
    int num;
    cout<<"Enter num";
    cin>>num;

    for (int i = 0; i < num; i++)
    {
       for (int j = 0; j <= i; j++)
       {
            cout<<j+1<<" ";
       }
       cout<<endl;
    }
    return 0;
}
*/

/*

1 2 3 4 5
1 2 3 4
1 2 3
1 2
1

int main(){
    int num;
    cout<<"Enter num";
    cin>>num;

    for (int i = 0; i < num; i++)
    {
       for (int j = 0; j < num - i; j++)
       {
            cout<<j+1<<" ";
       }
       cout<<endl;
    }

    return 0;
}

*/

// pattern 5
/*
   *
  * *
 * * * *
* * * * *



int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;

    for (int i = 1; i <= rows; i++)
    {
       for(int space=1;space<=rows-i;space++){
        cout<<" ";
       }

       for(int star=1;star<= 2*i-1;star++){
        cout<<"*";
       }

       cout<<endl;
    }


    return 0;
}

*/

// pattern 7
/*
* * * * * *
 * * * * *
  * * * *
   * * *
    * *
     *


int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;

    for (int i = rows; i >= 1; i--)
    {
       for(int space=1;space<= rows-i;space++){
        cout<<" ";
       }

       for(int star=1;star<= 2*i-1;star++){
        cout<<"*";
       }

       cout<<endl;
    }

    return 0;
}

*/

/*

        *
      * *
    * * *
  * * * *
* * * * *


int main(){

    int num;
    cout<<"Enter num";
    cin>>num;

    for (int i = 0; i<num; i++)
    {
        for(int space=0;space<num-i-1;space++){
            cout<<" ";
        }

        for (int star = 0; star < i+1; star++)
        {
            cout<<"*";
        }

        cout<<endl;

    }

    return 0;
}


*/

/*

*
**
***
****
*****
****
***
**
*




int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;


    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j <i+1; j++)
        {
            cout<<"*";
        }

        cout<<endl;
    }

    for (int i = 1; i < rows; i++)
    {
        for (int j = 0; j <rows-i; j++)
        {
            cout<<"*";
        }

        cout<<endl;
    }

    return 0;
}

*/

/*
    *
   **
  ***
 ****
*****
 ****
  ***
   **
    *




int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;


    for (int i = 1; i <= rows; i++)
    {
        for (int p = 0; p < rows-i; p++)
        {
           cout<<" ";
        }

        for (int j = 1; j <i+1; j++)
        {
            cout<<"*";
        }

        cout<<endl;
    }

    for (int i = 1; i < rows; i++)
    {
        for (int j = 0; j <i; j++)
        {
            cout<<" ";
        }

        for (int k = 0; k < rows-i; k++)
        {
         cout<<"*";
        }


        cout<<endl;
    }

    return 0;
}

*/

/*

    *
   ***
  *****
 *******
*********
 *******
  *****
   ***
    *



void printSpaces(int numSpaces) {
    for (int i = 0; i < numSpaces; i++) {
        std::cout << " ";
    }
}

void printStars(int numStars) {
    for (int i = 0; i < numStars; i++) {
        std::cout << "* ";
    }
    std::cout << std::endl;
}

void printDiamond(int n) {
    for (int i = 1; i <= n; i++) {
        printSpaces(n - i);
        printStars(2 * i - 1);
    }

    for (int i = n - 1; i >= 1; i--) {
        printSpaces(n - i);
        printStars(2 * i - 1);
    }
}

int main() {
    int n;

    cout << "Enter the number of rows (odd number): ";
    cin >> n;

    printDiamond(n);
    return 0;
}


*/

/*

1
11
121
1231
13541
148951

int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;

    int array[rows][rows]={0};

    for(int i=0;i<rows;i++){
        for(int j=0;j<i+1;j++){
            if(j==0 || i==j){
                array[i][j]=1;
                cout<<array[i][j]<<" ";
            }
            else{
                array[i][j]=array[i-1][j]+array[i-1][j-1];
                cout<<array[i][j]<<" ";
            }
        }
        cout<<endl;
    }

    return 0;
}

*/

/*

floyds triangle

1
2 3
4 5 6
7 8 9 10
11 12 13 14 15





int main(){
    static int num=1;
    int rows;
    cout<<"Enter rows";
    cin>>rows;

    for(int i=0;i<rows;i++){
        for(int j=0;j<i+1;j++){
            cout<<num<<" ";
            num++;
        }
        cout<<endl;
    }
}

*/


/*
Remove Duplicates From Array
*/

/*
void removeDuplicatesFromArray(unsigned int*,unsigned int);

int main(){

    unsigned int array[]={1,2,3,3,3,4,5,5,6,7,7,7};
    removeDuplicatesFromArray(array,(sizeof(array)/sizeof(array[0])));
    return 0;
}

void removeDuplicatesFromArray(unsigned int* array, unsigned int size){

    signed int idx=0;

    for(int i=1;i<size;i++){
        if(array[i]!=array[idx]){
            array[++idx]=array[i];
        }
    }

    for(int index=0;index<=idx;index++){
        cout<<array[index]<<" ";
    }
}
*/


// largest element in an array

/* 
int secondLargestElement(int *arr,int size){
    int i=0,firstLargestVar=-1,secondLargestVar=-1;
    while (i < size)
    {
       if(firstLargestVar<arr[i]){
        firstLargestVar=arr[i];
       }
        else if(secondLargestVar<arr[i]){
         secondLargestVar=arr[i];
       }
       i++;
    }
    return secondLargestVar;

}

int main(){
    int arr[]={999,953,5,6,7,23,543,87,23,543};
    int result = secondLargestElement(arr,(sizeof(arr)/sizeof(arr[0])));
    cout<<result<<endl;
    return 0;
}
*/


/* Left Rotatae array by 1 */

/*
void swapFunction(int *array,int startIndex,int endIndex){
    while (startIndex<endIndex)
    {
        int temp = array[startIndex];
        array[startIndex++] = array[endIndex];
        array[endIndex--] = temp;
    }
}
int *LeftRotatearray(int *arr, int size,int n){
    swapFunction(arr,0,n-1);
    swapFunction(arr,n,size-1);
    swapFunction(arr,0,size-1);
    return arr;
}
int main(){
    int arr[]={1,2,3,4,5};
    int *ans=LeftRotatearray(arr,sizeof(arr)/sizeof(arr[0]),3);
    for(int i=0;i<(sizeof(arr)/sizeof(arr[0]));i++){
        cout<<ans[i]<<" ";
    }
    return 0;
}
*/

/* Leader element in an array */

/*
void printLeaders(vector<int>& arr,int size){
    int LeaderTillNow=-1;
    vector<int> ans;
    for(int i=size-1;i>=0;i--){
        if(arr[i]>LeaderTillNow){
            ans.push_back(arr[i]);
            LeaderTillNow=arr[i];
        }
    }

    reverse(ans.begin(),ans.end());
    for(int i:ans){
        cout<<i<<" ";
    }
}
int main(){
    vector<int> array={7,10,4,3,6,5,2};
    printLeaders(array,array.size());
    return 0;
}

*/

/* Maximum Difference in an array such that j>i i.e arr[j]-arr[i] */

/*

int16_t maxDifference(vector<int>array){
    int minTillNow = INT_MAX;
    int maxTillNow = INT_MIN;

    for(int i:array){
        // minTillNow = min(minTillNow,i);
        // maxTillNow = max(maxTillNow,i-minTillNow);

        if(minTillNow>i){
            minTillNow=i;
        }
        if((i - minTillNow)>maxTillNow){
            maxTillNow=i - minTillNow;
        }
    }
    return maxTillNow;
}
int main(){
    vector<int> array={43,2,3,10,6,4,8,1};
    int16_t result=maxDifference(array);
    cout<<"Max Difference: "<<result<<endl;
    return 0;
}

*/

/* Frequency of Element in sorted array */

/*

int main()
{
    int arr[] = {1, 2, 2, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    int count = 1;
    for (int i = 1; i < n; i++)
    {
        if (arr[i] != arr[i - 1])
        {
            cout << arr[i - 1] << " occurs " << count << " times " << endl;
            count = 1;
        }
        else
        {
            count++;
        }

        if (i == n - 1)
        {
            cout << arr[i] << " occurs " << count << " times " << endl;
        }
    }
}

*/

/* STOCK BUY SELL - 1 */

/*

int maxProfit(vector<int>& prices){
    int maxProfit=0;
    for(int i=1; i<prices.size(); i++){
        if(prices[i]>prices[i-1]){
            maxProfit+=prices[i]-prices[i-1];
        }
    }
    return maxProfit;
}

int main(){
    vector<int> prices = {7, 1, 5, 3, 6, 4};
    int result = maxProfit(prices);
    cout<<"Maximum Profit: "<<result<<endl;
    return 0;
}

*/

/* Trapping Rain water problem */
/*
int maxWater(vector<int> &waterTank)
{
    vector<int> leftMax;
    vector<int> rightMax;
    int maxStoredWater=0;
    int maxLeft = waterTank[0];
    int maxRight = waterTank[waterTank.size() - 1];

    for (int i = 0; i < waterTank.size(); i++)
    {
        maxLeft = max(maxLeft, waterTank[i]);
        leftMax.push_back(maxLeft);
    }

    for (int i = (waterTank.size() - 1); i >= 0; i--)
    {
        maxRight = max(maxRight, waterTank[i]);
        rightMax.push_back(maxRight);
    }
    reverse(rightMax.begin(), rightMax.end());

    for (int i = 0; i < waterTank.size(); i++)
    {
        int tempmaxStoredWater = ((min(leftMax[i], rightMax[i]))-waterTank[i]);
        if (tempmaxStoredWater > 0)
        {
            maxStoredWater += tempmaxStoredWater;
        }
    }

    return maxStoredWater;
}

int main()
{
    vector<int> waterTank = {7, 1, 5, 3, 6, 4};
    int result = maxWater(waterTank);
    cout << "Maximum water stored : " << result << endl;
    return 0;
}
*/

/* Max Consecutive 1's in a binary array */
/*

int maxConsecutiveOne(vector<int>Arr){
    int count=0;
    int result=0;
    for(int element:Arr){
        (element != 0) ? count++ : count=0;
        result=max(result,count);
    }
    return result;
}

int main(){
    vector<int> binaryArray = {1,0,1,1,1,1,0,1,1,1,1,1};
    int result = maxConsecutiveOne(binaryArray);
    cout << "Maximum Consecutive 1's : " << result << endl;
    return 0;
}
*/

/* Maximum Subarray Sum */

// Naive Solution O(n2)

/*
int maxSubarraySum(vector<int> Arr)
{
    int result = 0;

    for (int i = 0; i < Arr.size(); i++)
    {
        int curr_max = Arr[i];
        for (int j = i + 1; j < Arr.size(); j++)
        {
            curr_max += Arr[j];
            result = max(result, curr_max);
        }
    }
    return result;
}
*/

// Efficient Solution O(n)

/*
int maxSubarraySum(vector<int> Arr)
{
    int result = 0;
    int curr_res = 0;

    for (int element : Arr)
    {
        curr_res = max(curr_res + element, element);
        result = max(curr_res, result);
    }

    return result;
}

int main()
{
    vector<int> binaryArray = {2, 3, -8, 7, -1, 2, 3};
    int result = maxSubarraySum(binaryArray);
    cout << "Maximum subarray sum : " << result << endl;
    return 0;
}

*/

/* Maximum Circular Subarray Sum */

/*
int maxSubarraySum(vector<int> Arr)
{
    int result = 0;
    int curr_res = 0;

    for (int element : Arr)
    {
        curr_res = max(curr_res + element, element);
        result = max(curr_res, result);
    }

    return result;
}

int maxSumInCircularSubArray(vector<int> arr)
{
    int MaxNormal = maxSubarraySum(arr);
    int result = 0;
    int arraySum = 0;

    if (MaxNormal < 0)
    {
        return MaxNormal;
    }

    for (int i = 0; i < arr.size(); i++)
    {
        arraySum += arr[i];
        arr[i] = -arr[i];
    }

    result = max((arraySum + maxSubarraySum(arr)), MaxNormal);
    return result;
}

int main()
{
    vector<int> Array = {-1, 40, -14, 7, 6, 5, -4, -1};
    int result = maxSumInCircularSubArray(Array);
    cout << "Maximum Circular Subarray Sum : " << result << endl;
    return 0;
}


*/

/*

int maxConsecutiveEvenOddSeq(vector<int> arr)
{
    int result = 0;
    int count = 1;

    for (int i = 1; i < arr.size(); i++)
    {
        if (((arr[i - 1] % 2 == 0) && (arr[i] % 2 != 0)) || ((arr[i - 1] % 2 != 0) && (arr[i] % 2 == 0)))
        {
            count++;
            result = max(result, count);
        }
        else
        {
            count = 1;
        }
    }

    return result;
}

int main()
{
    vector<int> Array = {12, 10, 2, 7, 4,2, 5};
    int result = maxConsecutiveEvenOddSeq(Array);
    cout << "Maximum Even Odd Consecutive Sequence : " << result << endl;
    return 0;
}

*/


// minimum no of flips to binary tree to make it look of same element

/*
void minimumNoOfFlips(vector<int> arr)
{
    int result = arr[0];

    for (int i = 1; i < arr.size();i++)
    {
        if (arr[i]!=result)
        {
            cout<<"Flip From "<<i<<"to";
            while( i!= (arr.size()) && arr[i]!=result) i++;
            cout<<(i-1)<<endl;
        }
    }
}

int main(){
    vector<int> Array = {0,1,1,0,1,0,0,0,0,0};
    minimumNoOfFlips(Array);
    return 0;
}
*/

// equilibrium point in an array is a point element from where left sumw is equal to right sum

// [2,4,5,6,54,11,6];
// 54 is equilibrium point element 

/*

void equilibriumPoint(vector<int> arr)
{
    int leftSum=0;
    int totalSum=0;

    for (int element:arr) totalSum+=element;

    for (int element:arr){
        if(leftSum == (totalSum - element)){
            cout<<"The Equilibrium Point is Element - "<<element;
        }
        else{
            leftSum+=element;
            totalSum-=element;
        }
    }
}

int main(){
    vector<int> Array = {2,4,5,6,54,11,6};
    equilibriumPoint(Array);
    return 0;
}

*/



// ###################### Searching ########################

// index of first occurence in sorted array logic would be same for last occurence

/*
void indexOfFirstOccurennce(int *arr,int size ,int element){
    int low=0;
    int high=size-1;

    while(low<=high){
        int mid = ((low + high) / 2);
        if(arr[mid]==element && ((mid) == 0 || arr[mid-1]!=arr[mid])){
           cout<<"The First Occurence of Element is at Index : "<<mid<<endl;
           break;
        }
        else if(arr[mid]>=element){
            high=mid-1;
        }
        else{
            low=mid+1;
        }
    }
}

void indexOfLastOccurennce(int *arr,int size,int element){
     int low=0;
    int high=size-1;

    while(low<=high){
        int mid = ((low + high) / 2);
        if(arr[mid]==element && (mid == (size-1) || arr[mid+1]!=arr[mid])){
           cout<<"The Last Occurence of Element is at Index : "<<mid<<endl;
           break;
        }
        else if(arr[mid]>element){
            high=mid-1;
        }
        else{
            low=mid+1;
        }
    }
}

int main(){
    int arr[]={10,20,20,40,60,70,75,75,82,90};
    int size=(sizeof(arr)/sizeof(arr[0]));
    indexOfFirstOccurennce(arr,size,75);
    indexOfLastOccurennce(arr,size,20);
    return 0;
}
*/

// search in sorted rotated array

/*

void searchInSortedRotated(int *arr,int size,int element){
    int low=0;
    int high=size-1;

    while(low<=high){
        int mid = ((low + high) / 2);
        if(arr[mid]==element){
           cout<<"The Element is at Index : "<<mid<<endl;
           break;
        }
        else if((arr[low]<=element) && (arr[mid]>element)){
            high=mid-1;
        }
        else{
            low=mid+1;
        }
    }
}

int main(){
    int arr[]={82,90,10,20,22,40,60,70,75,76};
    int size=(sizeof(arr)/sizeof(arr[0]));
    searchInSortedRotated(arr,size,22);
    return 0;
}

*/

// Problem: Check if a number is even or odd using bit manipulation.

int isEven(int num) {
    return (num & 1) == 0;   // last bit 0 means even 
}

// Problem: Count the number of set bits (1s) in an integer.

int countSetBits(int num) {
    int count = 0;
    while (num) {
        count += num & 1; // Increment count if last bit is 1
        num >>= 1;        // Right shift to check next bit
    }
    return count;
}

// Problem: Toggle the ith bit of a number.

int toggleIthBit(int num, int i) {
    return num ^ (1 << i); // XOR with 1 at ith position to toggle
}

// Problem: Set the ith bit of a number.

int setIthBit(int num, int i) {
    return num | (1 << i); // OR with 1 at ith position to set
}

// Problem: Clear the ith bit of a number.

int clearIthBit(int num, int i) {
    return num & ~(1 << i); // AND with NOT 1 at ith position to clear
}

// Problem: Check if the ith bit is set or not.

int isIthBitSet(int num, int i) {
    return (num & (1 << i)) != 0; // AND with 1 at ith position to check
}

// Problem: Find the position of the rightmost set bit.

int findRightmostSetBit(int num) {
    int position = 0;
    while (num) {
        if (num & 1) {
            return position;
        }
        num >>= 1;
        position++;
    }
    return -1; // No set bit found
}

// Problem: Find the position of the leftmost set bit.

int findLeftmostSetBit(unsigned int num) {
    int position = -1;
    int currentPos = 0;
    while (num) {
        if (num & 1) {
            position = currentPos;
        }
        num >>= 1;
        currentPos++;
    }
    return position; // Returns -1 if no set bit found
}

// Problem: Swap two numbers using bit manipulation.

void swap(int *a, int *b) {
    if (a != b) { // Check if the pointers are not the same
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}

// Problem: Reverse the bits of an integer.

unsigned int reverseBits(unsigned int num) {
    unsigned int reversed = 0;
    for (int i = 0; i < sizeof(num) * 8; i++) {
        reversed <<= 1;          // Shift reversed to left
        reversed |= (num & 1);  // Add last bit of num to reversed
        num >>= 1;              // Shift num to right
    }
    return reversed;
}

// Problem: Check if a number is a power of two.

int isPowerOfTwo(int num) {
    return num > 0 && (num & (num - 1)) == 0; // A power of two has only one set bit
}

// Problem: Count the number of bits required to convert integer A to integer B.

int countBitsToConvert(int a, int b) {
    int count = 0;
    int xor = a ^ b; // XOR will give bits that are different
    while (xor) {
        count += xor & 1; // Increment count if last bit is 1
        xor >>= 1;        // Right shift to check next bit
    }
    return count;
}

// Problem: Find the only non-repeating element in an array where every other element

int findNonRepeatingElement(int arr[], int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= arr[i]; // XORing all elements will cancel out repeating elements
    }
    return result;
}

// Problem: Generate all subsets of a set using bit manipulation.

void generateSubsets(int set[], int size) {
    int totalSubsets = 1 << size; // 2^size subsets
    for (int i = 0; i < totalSubsets; i++) {
        printf("{ ");
        for (int j = 0; j < size; j++) {
            if (i & (1 << j)) { // Check if jth bit is set
                printf("%d ", set[j]);
            }
        }
        printf("}\n");
    }
}

// Problem: Find the missing number in an array of size n-1 containing numbers

int findMissingNumber(int arr[], int size) {
    int n = size + 1; // Since one number is missing
    int totalXOR = 0;
    for (int i = 1; i <= n; i++) {
        totalXOR ^= i; // XOR of all numbers from 1 to n
    }
    int arrXOR = 0;
    for (int i = 0; i < size; i++) {
        arrXOR ^= arr[i]; // XOR of all elements in the array
    }
    return totalXOR ^ arrXOR; // Missing number is the XOR of the two results
}

// Problem: Count the number of bits to flip to convert A to B.

int countBitsToFlip(int a, int b) {
    int xor = a ^ b;
    int count = 0;
    while (xor) {
        count += xor & 1;
        xor >>= 1;
    }
    return count;
}

// Problem: Check if a number is a palindrome in binary representation.

int isBinaryPalindrome(int num) {
    int reversed = 0, original = num;
    while (num > 0) {
        reversed = (reversed << 1) | (num & 1);
        num >>= 1;
    }
    return original == reversed;
}

// Problem: Perform addition of two integers without using the '+' operator.

int addIntegers(int a, int b) {
    while (b != 0) {
        int carry = a & b; // Calculate carry
        a = a ^ b;         // Sum without carry
        b = carry << 1;    // Shift carry to the left
    }
    return a;
}

// Problem: Find the two non-repeating elements in an array where every other element

void findNonRepeatingElements(int arr[], int size) {
    int xorAll = 0;
    for (int i = 0; i < size; i++) {
        xorAll ^= arr[i]; // XOR of all elements
    }

    // Find a set bit (rightmost set bit)
    int setBit = xorAll & -xorAll;

    int num1 = 0, num2 = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] & setBit) {
            num1 ^= arr[i]; // Group with the set bit
        } else {
            num2 ^= arr[i]; // Group without the set bit
        }
    }

    printf("The two non-repeating elements are: %d and %d\n", num1, num2);  
}




/* 1 ) Largest Element in an array

#include <stdio.h>

int findLargest(int arr[],int size){
    int MAX_TILL_NOW=-1;

    for(int idx=0;idx<size;idx++){
        if(arr[idx]>MAX_TILL_NOW){
            MAX_TILL_NOW=arr[idx];
        }
    }
    return MAX_TILL_NOW;
}

int main(){
    int arr[]={23,76,89,43,56,90};
    int size=sizeof(arr)/sizeof(arr[0]);
    int largestElement=findLargest(arr,size);
    printf("The largest element is : %d",largestElement);
    return 0;
}

*/


/* 2 ) Second Largest Element in an array

#include <stdio.h>

int findSecLargestElement(int arr[],int size){
    int second_max=-1;
    int first_max=-1;

    for(int idx=0;idx<size;idx++){
        if(first_max<arr[idx]){
            second_max=first_max;
            first_max=arr[idx];
        }
        else if((second_max<arr[idx]) && (first_max != arr[idx])) {
                second_max=arr[idx];
        }
    }

    return second_max;
}
int main(){
    int arr[]={23,76,89,43,78,90};
    int size=sizeof(arr)/sizeof(arr[0]);
    int findSecLargest=findSecLargestElement(arr,size);
    printf("The second largest element is : %d",findSecLargest);
    return 0;
}

 */


/* 3 ) Check if array is sorted 

#include <stdio.h>

int isArraySorted(int *ptr,int size)
{
    for(int idx=0;idx<(size-1);idx++){
        if(*(ptr+idx) > *(ptr + idx + 1)){
            return 0;
        }
    }
    return 1;
}

int main(){
    int arr[]={23,76,89,43,78,90};
    int size=sizeof(arr)/sizeof(arr[0]);
    int sorted=isArraySorted(arr,size);
    (sorted == 1) ? printf("Sorted Array") :  printf("Not A Sorted Array");
    return 0;
}

*/


/* 4 ) Reverse an array 

#include<stdio.h>

void reverseArray(int *arr,int size){
    int st_idx=0;
    int end_idx=size-1;

    for(;st_idx<end_idx;st_idx++,end_idx--){
        int temp=arr[st_idx];
        arr[st_idx]=arr[end_idx];
        arr[end_idx]=temp;
    }
}

int main(){
    int arr[]={1,2,3,4,5,6};
    int size=sizeof(arr)/sizeof(arr[0]);
    reverseArray(arr,size);

    for(int idx=0;idx<size;idx++){
        printf("%d ",arr[idx]);
    }
    return 0;
}

*/



/*
  5 ) Remove Duplicate Element from sorted array 


#include <stdio.h>

int removeDuplicate(int *arr,int size){
    int sorted_idx=0;

    for(int idx=1;idx<size;idx++){
        if(arr[idx]!=arr[sorted_idx]){
            arr[++sorted_idx]=arr[idx];
        }
    }
    return sorted_idx;
}

int main(){
    int arr[]={1,2,3,3,4,5,6,6,8,8};
    int size=sizeof(arr)/sizeof(arr[0]);
    int newSize=removeDuplicate(arr,size);

    for(int idx=0;idx<newSize;idx++){
        printf("%d ",arr[idx]);
    }
    return 0;
}
*/

/*
  5 ) Remove Duplicate Element from unsorted array 


#include <stdio.h>

int removeDuplicate(int *arr,int size){
    int hashTable[1024];
    int newSize=0;
    
    for(int idx=0;idx<size;idx++){
        hashTable[arr[idx]]++;
    }

    for(int idx=0;idx<size;idx++){
        if(hashTable[arr[idx]] > 0){
            arr[newSize++]=arr[idx];
            hashTable[arr[idx]]=0;
        }
    }
    
    return newSize;
}

int main(){
    int arr[]={23,76,89,43,76,90,23,22,45,54,78,90};
    int size=sizeof(arr)/sizeof(arr[0]);
    int newSize=removeDuplicate(arr,size);

    for(int idx=0;idx<newSize;idx++){
        printf("%d ",arr[idx]);
    }
    return 0;
}

*/

/* 6 ) Left Rotate an Array by One 

#include <stdio.h>

void leftRotateByOne(int *arr,int size){
    int temp=arr[0];

    for(int idx=1;idx<size;idx++){
        arr[idx-1]=arr[idx];
    }
    arr[size-1]=temp;
}

int main(){
    int arr[]={1,2,3,4,5,6};
    int size=sizeof(arr)/sizeof(arr[0]);
    leftRotateByOne(arr,size);

    for(int idx=0;idx<size;idx++){
        printf("%d ",arr[idx]);
    }
    return 0;
}

*/

/* 7 ) Left Rotate an Array by D

#include <stdio.h>

void leftRotateByD(int *arr,int size,int d){
    for(int idx=0;idx<d;idx++){
        int temp=arr[0];

        for(int jdx=1;jdx<size;jdx++){
            arr[jdx-1]=arr[jdx];
        }
        arr[size-1]=temp;
    }
}

void reverseArray(int *arr,int size){
    int st_idx=0;
    int end_idx=size-1;

    for(;st_idx<end_idx;st_idx++,end_idx--){
        int temp=arr[st_idx];
        arr[st_idx]=arr[end_idx];
        arr[end_idx]=temp;
    }
}

void leftRotateByD(int *arr,int size,int d){
    reverseArray(arr,size);
    reverseArray(arr,size-d);
    reverseArray(arr+size-d,d);
}

int main(){
    int arr[]={1,2,3,4,5,6};
    int size=sizeof(arr)/sizeof(arr[0]);
    int d=2;
    leftRotateByD(arr,size,d);

    for(int idx=0;idx<size;idx++){
        printf("%d ",arr[idx]);
    }
    return 0;
}


*/

/* 8 ) Leaders in an Array 


#include <stdio.h>

void findLeaders(int *arr,int size){
    int max_till_now=-1;

    for(int idx=size-1;idx>=0;idx--){
        if(arr[idx]>max_till_now){
            printf("%d ",arr[idx]);
            max_till_now=arr[idx];
        }
    }
}

int main(){
    int arr[]={16,17,4,3,5,2};
    int size=sizeof(arr)/sizeof(arr[0]);
    findLeaders(arr,size);
    return 0;
}

*/


/* 10) Frequencies in a Sorted Array

#include <stdio.h>

void findFrequencies(int *arr,int size){
    int count=1;

    for(int idx=1;idx<size;idx++){
        if(arr[idx]==arr[idx-1]){
            count++;
        }
        else{
            printf("%d -> %d\n",arr[idx-1],count);
            count=1;
        }
    }
    printf("%d -> %d\n",arr[size-1],count);
}


int main(){
    int arr[]={10,10,20,20,20,30,30,40};
    int size=sizeof(arr)/sizeof(arr[0]);
    findFrequencies(arr,size);
    return 0;
}
*/


/* 11 ) Stock Buy and Sell Problem


#include <stdio.h>

void stockBuySell(int *arr,int size){
    for(int idx=1;idx<size;idx++){
        if(arr[idx]>arr[idx-1]){
            printf("Buy on day %d and Sell on day %d\n",idx-1,idx);
        }
    }
}

int main(){
    int arr[]={100,180,260,310,40,535,695};
    int size=sizeof(arr)/sizeof(arr[0]);
    stockBuySell(arr,size);
    return 0;
}

*/

/* 13 ) Trapping Rain Water

#include <stdio.h>

int trapRainWater(int *arr,int size){
    int left_max[size];
    int right_max[size];
    int total_water=0;

    left_max[0]=arr[0];
    for(int idx=1;idx<size;idx++){
        left_max[idx]=(arr[idx]>left_max[idx-1]) ? arr[idx] : left_max[idx-1];
    }

    right_max[size-1]=arr[size-1];
    for(int idx=size-2;idx>=0;idx--){
        right_max[idx]=(arr[idx]>right_max[idx+1]) ? arr[idx] : right_max[idx+1];
    }

    for(int idx=0;idx<size;idx++){
        total_water+=( (left_max[idx]<right_max[idx]) ? left_max[idx] : right_max[idx] ) - arr[idx];
    }

    return total_water;
}

int main(){
    int arr[]={0,1,0,2,1,0,1,3,2,1,2,1};
    int size=sizeof(arr)/sizeof(arr[0]);
    int totalWater=trapRainWater(arr,size);
    printf("Total water trapped is : %d",totalWater);
    return 0;
}

*/



/* 14 ) Maximum consecutive 1s 

#include <stdio.h>

int maxConsecutiveOnes(int *arr,int size){
    int count=0;
    int max_count=0;

    for(int idx=0;idx<size;idx++){
        if(arr[idx]==1){
            count++;
        }
        else{
            max_count=(count>max_count) ? count : max_count;
            count=0;
        }
    }
    max_count=(count>max_count) ? count : max_count;
    return max_count;
}


int main(){
    int arr[]={1,1,0,1,1,1};
    int size=sizeof(arr)/sizeof(arr[0]);
    int maxCount=maxConsecutiveOnes(arr,size);
    printf("Maximum consecutive 1s are : %d",maxCount);
    return 0;
}

*/


/* 15 ) Maximum subarray sum 

#include <stdio.h>

int maxSubarraySum(int *arr,int size){
    int max_sum=0;
    int current_sum=0;

    for(int idx=0;idx<size;idx++){
        current_sum+=arr[idx];
        if(current_sum>max_sum){
            max_sum=current_sum;
        }
        if(current_sum<0){
            current_sum=0;
        }
    }
    return max_sum;
}

int main(){
    int arr[]={5,4,-3,2,1};
    int size=sizeof(arr)/sizeof(arr[0]);
    int maxSum=maxSubarraySum(arr,size);
    printf("Maximum subarray sum is : %d",maxSum);
    return 0;
}

*/

/* 16 ) Longest Even Odd Subarray

#include <stdio.h>

int longestEvenOddSubarray(int *arr,int size){
    int max_length=1;
    int current_length=1;

    for(int idx=1;idx<size;idx++){
        if( (arr[idx]%2==0 && arr[idx-1]%2!=0) || (arr[idx]%2!=0 && arr[idx-1]%2==0) ){
            current_length++;
        }
        else{
            max_length=(current_length>max_length) ? current_length : max_length;
            current_length=1;
        }
    }
    max_length=(current_length>max_length) ? current_length : max_length;
    return max_length;
}

int main(){
    int arr[]={5,10,20,6,3,8};
    int size=sizeof(arr)/sizeof(arr[0]);
    int longestLength=longestEvenOddSubarray(arr,size);
    printf("Longest even odd subarray length is : %d",longestLength);
    return 0;
}

*/


/* 17 ) Maximum Circular Sum Subarray 

#include <stdio.h>

int maxSubarraySum(int *arr,int size){
    int max_sum=0;
    int current_sum=0;

    for(int idx=0;idx<size;idx++){
        current_sum+=arr[idx];
        if(current_sum>max_sum){
            max_sum=current_sum;
        }
        if(current_sum<0){
            current_sum=0;
        }
    }
    return max_sum;
}

int maxCircularSubarraySum(int *arr,int size){
    int max_kadane=maxSubarraySum(arr,size);
    int total_sum=0;

    for(int idx=0;idx<size;idx++){
        total_sum+=arr[idx];
        arr[idx]=-arr[idx];
    }

    int max_wrap=total_sum + maxSubarraySum(arr,size);
    return (max_wrap>max_kadane) ? max_wrap : max_kadane;
}

int main(){
    int arr[]={8,-8,9,-9,10,-11,12};
    int size=sizeof(arr)/sizeof(arr[0]);
    int maxCircularSum=maxCircularSubarraySum(arr,size);
    printf("Maximum circular subarray sum is : %d",maxCircularSum);
    return 0;
}

*/

/* 18 ) Majority Element 

#include <stdio.h>

int majorityElement(int *arr,int size){
    int count=1;
    int majority_index=0;

    for(int idx=1;idx<size;idx++){
        if(arr[idx]==arr[majority_index]){
            count++;
        }
        else{
            count--;
        }

        if(count==0){
            majority_index=idx;
            count=1;
        }
    }

    return arr[majority_index];
}

int main(){
    int arr[]={2,2,1,1,1,2,2};
    int size=sizeof(arr)/sizeof(arr[0]);
    int majority=majorityElement(arr,size);
    printf("The majority element is : %d",majority);
    return 0;
}


*/


/*
19 ) sort an array of 0s,1s and 2s 

#include <stdio.h>

void sortArray(int *arr,int size){
    int low=0;
    int mid=0;
    int high=size-1;

    while(mid<=high){
        if(arr[mid]==0){
            int temp=arr[low];
            arr[low]=arr[mid];
            arr[mid]=temp;
            low++;
            mid++;
        }
        else if(arr[mid]==1){
            mid++;
        }
        else{
            int temp=arr[mid];
            arr[mid]=arr[high];
            arr[high]=temp;
            high--;
        }
    }
}


int main(){
    int arr[]={0,1,2,0,1,2};
    int size=sizeof(arr)/sizeof(arr[0]);
    sortArray(arr,size);

    printf("Sorted array: ");
    for(int idx=0;idx<size;idx++){
        printf("%d ",arr[idx]);
    }
    return 0;
}

*/

/* 1 ) Reverse a Linked List 

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node* createNode(int data){
    struct Node *newNode=(struct Node*)malloc(sizeof(struct Node));
    newNode->data=data;
    newNode->next=NULL;
    return newNode;
}
1->2->3->4->5->NULL
void reverseLinkedList(struct Node **head){
    struct Node *prev=NULL;
    struct Node *current=*head;
    struct Node *next=NULL;

    while(current!=NULL){
        next=current->next;
        current->next=prev;
        prev=current;
        current=next;
    }
    *head=prev;
}

void printLinkedList(struct Node *head){
    struct Node *temp=head;
    while(temp!=NULL){
        printf("%d ",temp->data);
        temp=temp->next;
    }
}

int main(){
    struct Node *head=createNode(1);
    head->next=createNode(2);
    head->next->next=createNode(3);
    head->next->next->next=createNode(4);
    head->next->next->next->next=createNode(5);

    printf("Original Linked List: ");
    printLinkedList(head);

    reverseLinkedList(&head);

    printf("\nReversed Linked List: ");
    printLinkedList(head);
    return 0;
}

*/


/* 2 ) Detect a Cycle in a Linked List 

#include <stdio.h>

struct Node{
    int data;
    struct Node *next;
};

int hasCycle(struct Node *head){
    struct Node *slow=head;
    struct Node *fast=head;

    while(fast!=NULL && fast->next!=NULL){
        slow=slow->next;
        fast=fast->next->next;

        if(slow==fast){
            return 1;
        }
    }
    return 0;
}


int main(){
    struct Node node1={1,NULL};
    struct Node node2={2,NULL};
    struct Node node3={3,NULL};
    struct Node node4={4,NULL};

    node1.next=&node2;
    node2.next=&node3;
    node3.next=&node4;
    node4.next=&node2; // Creates a cycle

    if(hasCycle(&node1)){
        printf("Cycle detected in the linked list.");
    }
    else{
        printf("No cycle detected in the linked list.");
    }
    return 0;
}

*/



/* 3 ) Find the Middle of a Linked List */

/*

#include <stdio.h>

struct Node{
    int data;
    struct Node *next;
};


struct Node* findMiddle(struct Node *head){
    struct Node *slow=head;
    struct Node *fast=head;

    while(fast!=NULL && fast->next!=NULL){
        slow=slow->next;
        fast=fast->next->next;
    }
    return slow;
}


int main(){
    struct Node node1={1,NULL};
    struct Node node2={2,NULL};
    struct Node node3={3,NULL};
    struct Node node4={4,NULL};
    struct Node node5={5,NULL};

    node1.next=&node2;
    node2.next=&node3;
    node3.next=&node4;
    node4.next=&node5;

    struct Node *middle=findMiddle(&node1);
    printf("The middle element of the linked list is : %d",middle->data);
    return 0;
}

*/


/* 4 ) Remove Duplicates from a Linked List 

#include <stdio.h>
#include <stdlib.h>
struct Node{
    int data;
    struct Node *next;
};

void removeDuplicates(struct Node *head){
    struct Node *current=head;

    while(current!=NULL && current->next!=NULL){
        if(current->data==current->next->data){
            struct Node *temp=current->next;
            current->next=current->next->next;
            free(temp);
        }
        else{
            current=current->next;
        }
    }
}

int main(){
    struct Node node1={1,NULL};
    struct Node node2={1,NULL};
    struct Node node3={2,NULL};
    struct Node node4={3,NULL};
    struct Node node5={3,NULL};

    node1.next=&node2;
    node2.next=&node3;
    node3.next=&node4;
    node4.next=&node5;

    printf("Original Linked List: ");
    struct Node *temp=&node1;
    while(temp!=NULL){
        printf("%d ",temp->data);
        temp=temp->next;
    }

    removeDuplicates(&node1);

    printf("\nLinked List after removing duplicates: ");
    temp=&node1;
    while(temp!=NULL){
        printf("%d ",temp->data);
        temp=temp->next;
    }
    return 0;
}

*/


/* 5 ) Merge Two Sorted Linked Lists

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node* mergeSortedLists(struct Node *head1,struct Node *head2){
    struct Node dummy;
    struct Node *tail=&dummy;
    dummy.next=NULL;

    while(head1!=NULL && head2!=NULL){
        if(head1->data<head2->data){
            tail->next=head1;
            head1=head1->next;
        }
        else{
            tail->next=head2;
            head2=head2->next;
        }
        tail=tail->next;
    }

    if(head1!=NULL){
        tail->next=head1;
    }
    else{
        tail->next=head2;
    }

    return dummy.next;
}

int main(){
    struct Node node1={1,NULL};
    struct Node node2={3,NULL};
    struct Node node3={5,NULL};

    struct Node node4={2,NULL};
    struct Node node5={4,NULL};
    struct Node node6={6,NULL};

    node1.next=&node2;
    node2.next=&node3;

    node4.next=&node5;
    node5.next=&node6;

    struct Node *mergedHead=mergeSortedLists(&node1,&node4);

    printf("Merged Sorted Linked List: ");
    struct Node *temp=mergedHead;
    while(temp!=NULL){
        printf("%d ",temp->data);
        temp=temp->next;
    }
    return 0;
}

 */

/* 6 ) Intersection Point of Two Linked Lists 

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};


struct Node* findIntersection(struct Node *head1,struct Node *head2){
    struct Node *ptr1=head1;
    struct Node *ptr2=head2;

    while(ptr1!=ptr2){
        ptr1=(ptr1==NULL) ? head2 : ptr1->next;
        ptr2=(ptr2==NULL) ? head1 : ptr2->next;
    }
    return ptr1;
}


int main(){
    struct Node node1={1,NULL};
    struct Node node2={2,NULL};
    struct Node node3={3,NULL};

    struct Node node4={4,NULL};
    struct Node node5={5,NULL};

    node1.next=&node2;
    node2.next=&node3;

    node4.next=&node5;
    node5.next=&node2; // Creates an intersection at node2

    struct Node *intersection=findIntersection(&node1,&node4);
    if(intersection!=NULL){
        printf("The intersection point of the two linked lists is : %d",intersection->data);
    }
    else{
        printf("No intersection point found between the two linked lists.");
    }
    return 0;
}
*/


/* 7 ) Delete a Node in a Linked List 

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

void deleteNode(struct Node **head,struct Node *nodeToDelete){
    if(*head==NULL || nodeToDelete==NULL){
        return;
    }

    if(*head==nodeToDelete){
        *head=nodeToDelete->next;
    }
    else{
        struct Node *temp=*head;
        while(temp->next!=nodeToDelete){
            temp=temp->next;
        }
        temp->next=nodeToDelete->next;
    }
    free(nodeToDelete);
}


int main(){
    struct Node node1={1,NULL};
    struct Node node2={2,NULL};
    struct Node node3={3,NULL};
    struct Node node4={4,NULL};
    struct Node node5={5,NULL};

    node1.next=&node2;
    node2.next=&node3;
    node3.next=&node4;
    node4.next=&node5;

    printf("Original Linked List: ");
    struct Node *temp=&node1;
    while(temp!=NULL){
        printf("%d ",temp->data);
        temp=temp->next;
    }

    deleteNode(&node1,&node3);

    printf("\nLinked List after deleting node with value 3: ");
    temp=&node1;
    while(temp!=NULL){
        printf("%d ",temp->data);
        temp=temp->next;
    }
    return 0;
}

*/


/* 9 ) Check if a Linked List is a Palindrome 

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};


int isPalindrome(struct Node *head){
    struct Node *slow=head;
    struct Node *fast=head;
    struct Node *prev=NULL;
    struct Node *next=NULL;

    while(fast!=NULL && fast->next!=NULL){
        fast=fast->next->next;

        next=slow->next;
        slow->next=prev;
        prev=slow;
        slow=next;
    }

    if(fast!=NULL){
        slow=slow->next;
    }

    while(prev!=NULL && slow!=NULL){
        if(prev->data!=slow->data){
            return 0;
        }
        prev=prev->next;
        slow=slow->next;
    }
    return 1;
}

int main(){
    struct Node node1={1,NULL};
    struct Node node2={2,NULL};
    struct Node node3={3,NULL};
    struct Node node4={2,NULL};
    struct Node node5={1,NULL};

    node1.next=&node2;
    node2.next=&node3;
    node3.next=&node4;
    node4.next=&node5;

    if(isPalindrome(&node1)){
        printf("The linked list is a palindrome.");
    }
    else{
        printf("The linked list is not a palindrome.");
    }
    return 0;
}
*/


/* 10 ) Rotate a Linked List

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

void rotateLinkedList(struct Node **head,int k){
    if(*head==NULL || k==0){
        return;
    }

    struct Node *current=*head;
    int count=1;

    while(count<k && current!=NULL){
        current=current->next;
        count++;
    }

    if(current==NULL){
        return;
    }

    struct Node *kthNode=current;

    while(current->next!=NULL){
        current=current->next;
    }
    current->next=*head;
    *head=kthNode->next;
    kthNode->next=NULL;
}

int main(){
    struct Node node1={1,NULL};
    struct Node node2={2,NULL};
    struct Node node3={3,NULL};
    struct Node node4={4,NULL};
    struct Node node5={5,NULL};

    node1.next=&node2;
    node2.next=&node3;
    node3.next=&node4;
    node4.next=&node5;

    int k=2;
    rotateLinkedList(&node1,k);

    printf("Linked List after rotating by %d positions: ",k);
    struct Node *temp=&node3; // New head after rotation
    while(temp!=NULL){
        printf("%d ",temp->data);
        temp=temp->next;
    }
    return 0;
}
*/


/* 14 ) Reverse Nodes in k-Group

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node* reverseKGroup(struct Node *head,int k){
    struct Node *current=head;
    struct Node *prev=NULL;
    struct Node *next=NULL;
    int count=0;

    while(current!=NULL && count<k){
        next=current->next;
        current->next=prev;
        prev=current;
        current=next;
        count++;
    }

    if(next!=NULL){
        head->next=reverseKGroup(next,k);
    }
    return prev;
}


int main(){
    struct Node node1={1,NULL};
    struct Node node2={2,NULL};
    struct Node node3={3,NULL};
    struct Node node4={4,NULL};
    struct Node node5={5,NULL};

    node1.next=&node2;
    node2.next=&node3;
    node3.next=&node4;
    node4.next=&node5;

    int k=2;
    struct Node *newHead=reverseKGroup(&node1,k);

    printf("Linked List after reversing in groups of %d: ",k);
    struct Node *temp=newHead;
    while(temp!=NULL){
        printf("%d ",temp->data);
        temp=temp->next;
    }
    return 0;
}

 */




// #include <bits/stdc++.h>
// using namespace std;

// 1. Prime no

/*
bool isPrime(int num){
    if(num<=1){
        return false;
    }

    for (int i = 2; i <= sqrt(num); i++)
    
       if(num%i==0){
        return false;
       }
    }

    return true;
}

int main(){
    int num;
    cout<<"Enter no to check Prime or not";
    cin>>num;
    isPrime(num)?cout<<"Prime":cout<<"Not Prime";
    return 0;
}
*/

// 2. Fibonacci series

/*
void fibonacci(int num){
    int num1=0;
    int num2=1;
    int sum;
    cout<<num1<<" "<<num2<<" ";

    for(int i=0;i<num-2;i++){
        sum=num1+num2;
        cout<<sum<<" ";
        num1=num2;
        num2=sum;
    }
}

int main(){
    int num;
    cout<<"Enter no to print fibonacci series";
    cin>>num;
    fibonacci(num);
    return 0;
}
*/

// 3. Pallindrome no

/*
bool checkPallindrom(int num){
    string converted=to_string(num);
    string rev=converted;
    reverse(rev.begin(),rev.end());
    return rev==converted ? true : false;
}



bool checkPallindrom(int num)
{
    int revNum = 0;
    int temp;
    int orginalNum = num;

    while (num)
    {
        temp = num % 10;
        revNum = revNum * 10 + temp;
        num = num / 10;
    }

    return revNum == orginalNum;
}

int main()
{
    int num;
    cout << "Enter no to check whether it is pallindrome or not ? ";
    cin >> num;
    if (checkPallindrom(num))
    {
        cout << "Pallindrome";
    }
    else
    {
        cout << "Not Pallindrome";
    }
    return 0;
}

*/





// lcm and gcd of numbers

/*
int gcd(int num1,int num2){
    if(num2==0){
        return num1;
    }
    return gcd(num2,num1%num2);

    ||


    while (num2 != 0) {
        int temp = num2;
        num2 = num1 % num2;
        num1 = temp;
    }
    return num1;
}

int lcm(int num1,int num2){
    return (num1*num2)/gcd(num1,num2);
}

int main(){
    int num1,num2;
    cout<<"Enter two numbers";
    cin>>num1>>num2;
    cout<<"The gcd of two numbers is : "<<gcd(num1,num2);
    cout<<"The lcm of two numbers is : "<<lcm(num1,num2);
    return 0;
}
*/





// factorial
/*
int factorial(int num){
    if(num==1){
        return 1;
    }
    return num*factorial(num-1);
}

int main(){
    int num;
    cout<<"Enter no to find Factorial";
    cin>>num;
    cout<<"The factorial of no is : "<<factorial(num);
    return 0;
}
*/

// 4. Armstrong no

/*
unsigned int cube(unsigned int num)
{
    return num * num * num;
}

bool Armstrong(int num)
{
    int originalNum = num;
    int sum=0;
    int temp;
    while (num)
    {
        temp = num % 10;
        sum += cube(temp);
        num = num / 10;
    }
    return sum == originalNum ? true : false;
}

int main()
{
    int num;
    cout << "Enter no to find Armstrong or not";
    cin >> num;
    cout <<"\n The no is"<<Armstrong(num);
    return 0;
}

*/

// decimal to binary

/*
void decimalToBinary(int decimal) {
    if (decimal == 0) {
        cout << "0";
        return;
    }

    int binary[32];
    int i = 0;

    while (decimal) {
        binary[i] = decimal % 2;
        decimal /= 2;
        i++;
    }

    for (int j = i - 1; j >= 0; j--) {
        cout << binary[j];
    }
}

int main() {
    int decimal;
    cout << "Enter a decimal number: ";
    cin >> decimal;

    cout << "Binary representation: ";
    decimalToBinary(decimal);

    return 0;
}
*/

// A year is a leap year if the following conditions are satisfied:

// The year is multiple of 400.
// The year is multiple of 4 and not multiple of 100.

// Anagram
/*
bool anagram(string st1,string st2){
    if(st1.length()!=st2.length()){
        return false;
    }

    int count[256]={0};

    for(int i=0;i<st1.length();i++){
        count[st1[i]]++;
        count[st2[i]]--;
    }

    for(int i=0;i<256;i++){
        if(count[i]!=0){
            return false;
        }
    }
    return true;
}


int main(){
    string st1="listen";
    string st2="silent";

    if(anagram(st1,st2)){
        cout<<"Anagram";
    }
    else{
        cout<<"Not Anagram";
    }
return 0;
}
*/

// program for malloc and calloc in c++
/*
int main(){
    int *ptr;
    int n;
    cout<<"Enter no of elements";
    cin>>n;
    ptr=(int*)malloc(n*sizeof(int));
    if(ptr==NULL){
        cout<<"Memory not allocated";
    }
    else{
        cout<<"Memory allocated successfully";
        for(int i=0;i<n;i++){
            ptr[i]=i+1;
        }
        cout<<"The elements of array are : ";
        for(int i=0;i<n;i++){
            cout<<ptr[i]<<" ";
        }
    }
    return 0;
}

*/

// program for calloc
/*
int main(){
    int *ptr;
    int n;
    cout<<"Enter no of elements";
    cin>>n;
    ptr=(int*)calloc(n,sizeof(int));
    if(ptr==NULL){
        cout<<"Memory not allocated";
    }
    else{
        cout<<"Memory allocated successfully";
        for(int i=0;i<n;i++){
            ptr[i]=i+1;
        }
        cout<<"The elements of array are : ";
        for(int i=0;i<n;i++){
            cout<<ptr[i]<<" ";
        }
    }
    return 0;
}
*/

// structure pointer

/*
struct Person {
    string name;
    int age;
};

int main() {

    Person person;
    Person* personPtr;
    personPtr = &person;
    personPtr->name = "John Doe";
    personPtr->age = 25;

    cout << "Name: " << personPtr->name << endl;
    cout << "Age: " << personPtr->age << endl;

    return 0;
}
*/

// Declaring A Pointer To A Function

/*
int addition(int num1,int num2){
    return num1+num2;
}

int main(){
    int (*ptr)(int,int)=addition; // Declaring A Pointer To A Function
    cout<<(*ptr)(2,3);  // Calling A Function Through Function Pointer
    return -1;
}
*/

// Passing a Function's Address as an Argument to Other Function

/*
int addition(int num1,int num2){
    return num1+num2;
}

void print(int (*ptr)(int,int)){
    cout<<(*ptr)(2,3);
}

int main(){
    print(addition);
    return -1;
}
*/

// functional array pointer

/*
int addition(int num1,int num2){
    return num1+num2;
}

int subtraction(int num1,int num2){
    return num1-num2;
}

int multiplication(int num1,int num2){
    return num1*num2;
}

int division(int num1,int num2){
    return num1/num2;
}

int main(){
    int (*ptr[4])(int,int)={addition,subtraction,multiplication,division};
    cout<<ptr[0](2,3)<<endl;
    cout<<ptr[1](2,3)<<endl;
    cout<<ptr[2](2,3)<<endl;
    cout<<ptr[3](2,3)<<endl;
    return -1;
}
*/

/*

union Data
{
    int intValue;
    float floatValue;
    char charValue;
};

int main()
{
    Data myData ;

    myData.intValue = 42;
    std::cout << "Integer value: " << myData.intValue << std::endl;

    myData.floatValue = 3.14;
    std::cout << "Float value: " << myData.floatValue << std::endl;
    std::cout << "Integer value: " << myData.intValue << std::endl; // Accessing intValue after modifying floatValue

    myData.charValue = 'A';
    std::cout << "Character value: " << myData.charValue << std::endl;
    std::cout << "Float value: " << myData.floatValue << std::endl; // Accessing floatValue after modifying charValue

    return 0;
}

*/

// pattern 1

/*

*
* *
* * *
* * * *
* * * * *



int main(){
    char arr[5][5];

    for(int i=0;i<5;i++){
        for(int j=0;j<=i;j++){
            arr[i][j]='*';
        }
    }

    cout<<"Printing pattern \n";

    for(int i=0;i<5;i++){
        for(int j=0;j<=i;j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }


    return -1;
}

*/

// Pattern 2

/*

* * * * *
  * * * *
    * * *
      * *
        *




int main(){
    char arr[5][5];

    for(int i=0;i<5;i++){
        for (int k = 0;k<i; k++)
        {
            arr[i][k]=' ';
        }

        for(int j=i;j<5;j++){
            arr[i][j]='*';
        }
    }

    cout<<"Printing pattern \n";

    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }


    return -1;
}


*/

// pattern 3

/*
* * * * *
* * * *
* * *
* *
*





int main(){
    char arr[5][5];

    for(int i=0;i<5;i++){
        for(int j=0;j<5-i;j++){
            arr[i][j]='*';
        }
    }

    cout<<"Printing pattern \n";

      for(int i=0;i<5;i++){
        for(int j=0;j<5-i;j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
    return -1;
}

*/

// pattern 4

/*
1
1 2
1 2 3
1 2 3 4
1 2 3 4 5

int main(){
    int num;
    cout<<"Enter num";
    cin>>num;

    for (int i = 0; i < num; i++)
    {
       for (int j = 0; j <= i; j++)
       {
            cout<<j+1<<" ";
       }
       cout<<endl;
    }
    return 0;
}
*/

/*

1 2 3 4 5
1 2 3 4
1 2 3
1 2
1

int main(){
    int num;
    cout<<"Enter num";
    cin>>num;

    for (int i = 0; i < num; i++)
    {
       for (int j = 0; j < num - i; j++)
       {
            cout<<j+1<<" ";
       }
       cout<<endl;
    }

    return 0;
}

*/

// pattern 5
/*
   *
  * *
 * * * *
* * * * *



int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;

    for (int i = 1; i <= rows; i++)
    {
       for(int space=1;space<=rows-i;space++){
        cout<<" ";
       }

       for(int star=1;star<= 2*i-1;star++){
        cout<<"*";
       }

       cout<<endl;
    }


    return 0;
}

*/

// pattern 7
/*
* * * * * *
 * * * * *
  * * * *
   * * *
    * *
     *


int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;

    for (int i = rows; i >= 1; i--)
    {
       for(int space=1;space<= rows-i;space++){
        cout<<" ";
       }

       for(int star=1;star<= 2*i-1;star++){
        cout<<"*";
       }

       cout<<endl;
    }

    return 0;
}

*/

/*

        *
      * *
    * * *
  * * * *
* * * * *


int main(){

    int num;
    cout<<"Enter num";
    cin>>num;

    for (int i = 0; i<num; i++)
    {
        for(int space=0;space<num-i-1;space++){
            cout<<" ";
        }

        for (int star = 0; star < i+1; star++)
        {
            cout<<"*";
        }

        cout<<endl;

    }

    return 0;
}


*/

/*

*
**
***
****
*****
****
***
**
*




int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;


    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j <i+1; j++)
        {
            cout<<"*";
        }

        cout<<endl;
    }

    for (int i = 1; i < rows; i++)
    {
        for (int j = 0; j <rows-i; j++)
        {
            cout<<"*";
        }

        cout<<endl;
    }

    return 0;
}

*/

/*
    *
   **
  ***
 ****
*****
 ****
  ***
   **
    *




int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;


    for (int i = 1; i <= rows; i++)
    {
        for (int p = 0; p < rows-i; p++)
        {
           cout<<" ";
        }

        for (int j = 1; j <i+1; j++)
        {
            cout<<"*";
        }

        cout<<endl;
    }

    for (int i = 1; i < rows; i++)
    {
        for (int j = 0; j <i; j++)
        {
            cout<<" ";
        }

        for (int k = 0; k < rows-i; k++)
        {
         cout<<"*";
        }


        cout<<endl;
    }

    return 0;
}

*/

/*

    *
   ***
  *****
 *******
*********
 *******
  *****
   ***
    *



void printSpaces(int numSpaces) {
    for (int i = 0; i < numSpaces; i++) {
        std::cout << " ";
    }
}

void printStars(int numStars) {
    for (int i = 0; i < numStars; i++) {
        std::cout << "* ";
    }
    std::cout << std::endl;
}

void printDiamond(int n) {
    for (int i = 1; i <= n; i++) {
        printSpaces(n - i);
        printStars(2 * i - 1);
    }

    for (int i = n - 1; i >= 1; i--) {
        printSpaces(n - i);
        printStars(2 * i - 1);
    }
}

int main() {
    int n;

    cout << "Enter the number of rows (odd number): ";
    cin >> n;

    printDiamond(n);
    return 0;
}


*/

/*

1
11
121
1231
13541
148951

int main(){
    int rows;
    cout<<"Enter rows";
    cin>>rows;

    int array[rows][rows]={0};

    for(int i=0;i<rows;i++){
        for(int j=0;j<i+1;j++){
            if(j==0 || i==j){
                array[i][j]=1;
                cout<<array[i][j]<<" ";
            }
            else{
                array[i][j]=array[i-1][j]+array[i-1][j-1];
                cout<<array[i][j]<<" ";
            }
        }
        cout<<endl;
    }

    return 0;
}

*/

/*

floyds triangle

1
2 3
4 5 6
7 8 9 10
11 12 13 14 15





int main(){
    static int num=1;
    int rows;
    cout<<"Enter rows";
    cin>>rows;

    for(int i=0;i<rows;i++){
        for(int j=0;j<i+1;j++){
            cout<<num<<" ";
            num++;
        }
        cout<<endl;
    }
}

*/


/*
Remove Duplicates From Array
*/

/*
void removeDuplicatesFromArray(unsigned int*,unsigned int);

int main(){

    unsigned int array[]={1,2,3,3,3,4,5,5,6,7,7,7};
    removeDuplicatesFromArray(array,(sizeof(array)/sizeof(array[0])));
    return 0;
}

void removeDuplicatesFromArray(unsigned int* array, unsigned int size){

    signed int idx=0;

    for(int i=1;i<size;i++){
        if(array[i]!=array[idx]){
            array[++idx]=array[i];
        }
    }

    for(int index=0;index<=idx;index++){
        cout<<array[index]<<" ";
    }
}
*/


// largest element in an array

/* 
int secondLargestElement(int *arr,int size){
    int i=0,firstLargestVar=-1,secondLargestVar=-1;
    while (i < size)
    {
       if(firstLargestVar<arr[i]){
            secondLargestVar=firstLargestVar;
            firstLargestVar=arr[i];
       }
        else if(secondLargestVar<arr[i] && arr[i]!=firstLargestVar){
         secondLargestVar=arr[i];
       }
       i++;
    }
    return secondLargestVar;

}

int main(){
    int arr[]={999,953,5,6,7,23,543,87,23,543};
    int result = secondLargestElement(arr,(sizeof(arr)/sizeof(arr[0])));
    cout<<result<<endl;
    return 0;
}
*/


/* Left Rotatae array by 1 */

/*
void swapFunction(int *array,int startIndex,int endIndex){
    while (startIndex<endIndex)
    {
        int temp = array[startIndex];
        array[startIndex++] = array[endIndex];
        array[endIndex--] = temp;
    }
}
int *LeftRotatearray(int *arr, int size,int n){
    swapFunction(arr,0,n-1);
    swapFunction(arr,n,size-1);
    swapFunction(arr,0,size-1);
    return arr;
}
int main(){
    int arr[]={1,2,3,4,5};
    int *ans=LeftRotatearray(arr,sizeof(arr)/sizeof(arr[0]),3);
    for(int i=0;i<(sizeof(arr)/sizeof(arr[0]));i++){
        cout<<ans[i]<<" ";
    }
    return 0;
}
*/

/* Leader element in an array */

/*
void printLeaders(vector<int>& arr,int size){
    int LeaderTillNow=-1;
    vector<int> ans;
    for(int i=size-1;i>=0;i--){
        if(arr[i]>LeaderTillNow){
            ans.push_back(arr[i]);
            LeaderTillNow=arr[i];
        }
    }

    reverse(ans.begin(),ans.end());
    for(int i:ans){
        cout<<i<<" ";
    }
}
int main(){
    vector<int> array={7,10,4,3,6,5,2};
    printLeaders(array,array.size());
    return 0;
}

*/

/* Maximum Difference in an array such that j>i i.e arr[j]-arr[i] */

/*

int16_t maxDifference(vector<int>array){
    int minTillNow = INT_MAX;
    int maxTillNow = INT_MIN;

    for(int i:array){
        // minTillNow = min(minTillNow,i);
        // maxTillNow = max(maxTillNow,i-minTillNow);

        if(minTillNow>i){
            minTillNow=i;
        }
        if((i - minTillNow)>maxTillNow){
            maxTillNow=i - minTillNow;
        }
    }
    return maxTillNow;
}
int main(){
    vector<int> array={43,2,3,10,6,4,8,1};
    int16_t result=maxDifference(array);
    cout<<"Max Difference: "<<result<<endl;
    return 0;
}

*/

/* Frequency of Element in sorted array */

/*

int main()
{
    int arr[] = {1, 2, 2, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    int count = 1;
    for (int i = 1; i < n; i++)
    {
        if (arr[i] != arr[i - 1])
        {
            cout << arr[i - 1] << " occurs " << count << " times " << endl;
            count = 1;
        }
        else
        {
            count++;
        }

        if (i == n - 1)
        {
            cout << arr[i] << " occurs " << count << " times " << endl;
        }
    }
}

*/

/* STOCK BUY SELL - 1 */

/*

int maxProfit(vector<int>& prices){
    int maxProfit=0;
    for(int i=1; i<prices.size(); i++){
        if(prices[i]>prices[i-1]){
            maxProfit+=prices[i]-prices[i-1];
        }
    }
    return maxProfit;
}

int main(){
    vector<int> prices = {7, 1, 5, 3, 6, 4};
    int result = maxProfit(prices);
    cout<<"Maximum Profit: "<<result<<endl;
    return 0;
}

*/

/* Trapping Rain water problem */
/*
int maxWater(vector<int> &waterTank)
{
    vector<int> leftMax;
    vector<int> rightMax;
    int maxStoredWater=0;
    int maxLeft = waterTank[0];
    int maxRight = waterTank[waterTank.size() - 1];

    for (int i = 0; i < waterTank.size(); i++)
    {
        maxLeft = max(maxLeft, waterTank[i]);
        leftMax.push_back(maxLeft);
    }

    for (int i = (waterTank.size() - 1); i >= 0; i--)
    {
        maxRight = max(maxRight, waterTank[i]);
        rightMax.push_back(maxRight);
    }
    reverse(rightMax.begin(), rightMax.end());

    for (int i = 0; i < waterTank.size(); i++)
    {
        int tempmaxStoredWater = ((min(leftMax[i], rightMax[i]))-waterTank[i]);
        if (tempmaxStoredWater > 0)
        {
            maxStoredWater += tempmaxStoredWater;
        }
    }

    return maxStoredWater;
}

int main()
{
    vector<int> waterTank = {7, 1, 5, 3, 6, 4};
    int result = maxWater(waterTank);
    cout << "Maximum water stored : " << result << endl;
    return 0;
}
*/

/* Max Consecutive 1's in a binary array */
/*

int maxConsecutiveOne(vector<int>Arr){
    int count=0;
    int result=0;
    for(int element:Arr){
        (element != 0) ? count++ : count=0;
        result=max(result,count);
    }
    return result;
}

int main(){
    vector<int> binaryArray = {1,0,1,1,1,1,0,1,1,1,1,1};
    int result = maxConsecutiveOne(binaryArray);
    cout << "Maximum Consecutive 1's : " << result << endl;
    return 0;
}
*/

/* Maximum Subarray Sum */

// Naive Solution O(n2)

/*
int maxSubarraySum(vector<int> Arr)
{
    int result = 0;

    for (int i = 0; i < Arr.size(); i++)
    {
        int curr_max = Arr[i];
        for (int j = i + 1; j < Arr.size(); j++)
        {
            curr_max += Arr[j];
            result = max(result, curr_max);
        }
    }
    return result;
}
*/

// Efficient Solution O(n)

/*
int maxSubarraySum(vector<int> Arr)
{
    int result = 0;
    int curr_res = 0;

    for (int element : Arr)
    {
        curr_res = max(curr_res + element, element);
        result = max(curr_res, result);
    }

    return result;
}

int main()
{
    vector<int> binaryArray = {2, 3, -8, 7, -1, 2, 3};
    int result = maxSubarraySum(binaryArray);
    cout << "Maximum subarray sum : " << result << endl;
    return 0;
}

*/

/* Maximum Circular Subarray Sum */

/*
int maxSubarraySum(vector<int> Arr)
{
    int result = 0;
    int curr_res = 0;

    for (int element : Arr)
    {
        curr_res = max(curr_res + element, element);
        result = max(curr_res, result);
    }

    return result;
}

int maxSumInCircularSubArray(vector<int> arr)
{
    int MaxNormal = maxSubarraySum(arr);
    int result = 0;
    int arraySum = 0;

    if (MaxNormal < 0)
    {
        return MaxNormal;
    }

    for (int i = 0; i < arr.size(); i++)
    {
        arraySum += arr[i];
        arr[i] = -arr[i];
    }

    result = max((arraySum + maxSubarraySum(arr)), MaxNormal);
    return result;
}

int main()
{
    vector<int> Array = {-1, 40, -14, 7, 6, 5, -4, -1};
    int result = maxSumInCircularSubArray(Array);
    cout << "Maximum Circular Subarray Sum : " << result << endl;
    return 0;
}


*/

/*

int maxConsecutiveEvenOddSeq(vector<int> arr)
{
    int result = 0;
    int count = 1;

    for (int i = 1; i < arr.size(); i++)
    {
        if (((arr[i - 1] % 2 == 0) && (arr[i] % 2 != 0)) || ((arr[i - 1] % 2 != 0) && (arr[i] % 2 == 0)))
        {
            count++;
            result = max(result, count);
        }
        else
        {
            count = 1;
        }
    }

    return result;
}

int main()
{
    vector<int> Array = {12, 10, 2, 7, 4,2, 5};
    int result = maxConsecutiveEvenOddSeq(Array);
    cout << "Maximum Even Odd Consecutive Sequence : " << result << endl;
    return 0;
}

*/


// minimum no of flips to binary tree to make it look of same element

/*
void minimumNoOfFlips(vector<int> arr)
{
    int result = arr[0];

    for (int i = 1; i < arr.size();i++)
    {
        if (arr[i]!=result)
        {
            cout<<"Flip From "<<i<<"to";
            while( i!= (arr.size()) && arr[i]!=result) i++;
            cout<<(i-1)<<endl;
        }
    }
}

int main(){
    vector<int> Array = {0,1,1,0,1,0,0,0,0,0};
    minimumNoOfFlips(Array);
    return 0;
}
*/

// equilibrium point in an array is a point element from where left sumw is equal to right sum

// [2,4,5,6,54,11,6];
// 54 is equilibrium point element 

/*

void equilibriumPoint(vector<int> arr)
{
    int leftSum=0;
    int totalSum=0;

    for (int element:arr) totalSum+=element;

    for (int element:arr){
        if(leftSum == (totalSum - element)){
            cout<<"The Equilibrium Point is Element - "<<element;
        }
        else{
            leftSum+=element;
            totalSum-=element;
        }
    }
}

int main(){
    vector<int> Array = {2,4,5,6,54,11,6};
    equilibriumPoint(Array);
    return 0;
}

*/



// ###################### Searching ########################

// index of first occurence in sorted array logic would be same for last occurence

/*
void indexOfFirstOccurennce(int *arr,int size ,int element){
    int low=0;
    int high=size-1;

    while(low<=high){
        int mid = ((low + high) / 2);
        if(arr[mid]==element && ((mid) == 0 || arr[mid-1]!=arr[mid])){
           cout<<"The First Occurence of Element is at Index : "<<mid<<endl;
           break;
        }
        else if(arr[mid]>=element){
            high=mid-1;
        }
        else{
            low=mid+1;
        }
    }
}

void indexOfLastOccurennce(int *arr,int size,int element){
     int low=0;
    int high=size-1;

    while(low<=high){
        int mid = ((low + high) / 2);
        if(arr[mid]==element && (mid == (size-1) || arr[mid+1]!=arr[mid])){
           cout<<"The Last Occurence of Element is at Index : "<<mid<<endl;
           break;
        }
        else if(arr[mid]>element){
            high=mid-1;
        }
        else{
            low=mid+1;
        }
    }
}

int main(){
    int arr[]={10,20,20,40,60,70,75,75,82,90};
    int size=(sizeof(arr)/sizeof(arr[0]));
    indexOfFirstOccurennce(arr,size,75);
    indexOfLastOccurennce(arr,size,20);
    return 0;
}
*/

// search in sorted rotated array

/*

void searchInSortedRotated(int *arr,int size,int element){
    int low=0;
    int high=size-1;

    while(low<=high){
        int mid = ((low + high) / 2);
        if(arr[mid]==element){
           cout<<"The Element is at Index : "<<mid<<endl;
           break;
        }
        else if((arr[low]<=element) && (arr[mid]>element)){
            high=mid-1;
        }
        else{
            low=mid+1;
        }
    }
}

int main(){
    int arr[]={82,90,10,20,22,40,60,70,75,76};
    int size=(sizeof(arr)/sizeof(arr[0]));
    searchInSortedRotated(arr,size,22);
    return 0;
}

*/

// Problem: Check if a number is even or odd using bit manipulation.

int isEven(int num) {
    return (num & 1) == 0;   // last bit 0 means even 
}

// Problem: Count the number of set bits (1s) in an integer.

int countSetBits(int num) {
    int count = 0;
    while (num) {
        count += num & 1; // Increment count if last bit is 1
        num >>= 1;        // Right shift to check next bit
    }
    return count;
}

// Problem: Toggle the ith bit of a number.

int toggleIthBit(int num, int i) {
    return num ^ (1 << i); // XOR with 1 at ith position to toggle
}

// Problem: Set the ith bit of a number.

int setIthBit(int num, int i) {
    return num | (1 << i); // OR with 1 at ith position to set
}

// Problem: Clear the ith bit of a number.

int clearIthBit(int num, int i) {
    return num & ~(1 << i); // AND with NOT 1 at ith position to clear
}

// Problem: Check if the ith bit is set or not.

int isIthBitSet(int num, int i) {
    return (num & (1 << i)) != 0; // AND with 1 at ith position to check
}

// Problem: Find the position of the rightmost set bit.

int findRightmostSetBit(int num) {
    int position = 0;
    while (num) {
        if (num & 1) {
            return position;
        }
        num >>= 1;
        position++;
    }
    return -1; // No set bit found
}

// Problem: Find the position of the leftmost set bit.

int findLeftmostSetBit(unsigned int num) {
    int position = -1;
    int currentPos = 0;
    while (num) {
        if (num & 1) {
            position = currentPos;
        }
        num >>= 1;
        currentPos++;
    }
    return position; // Returns -1 if no set bit found
}

// Problem: Swap two numbers using bit manipulation.

void swap(int *a, int *b) {
    if (a != b) { // Check if the pointers are not the same
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}

// Problem: Reverse the bits of an integer.

unsigned int reverseBits(unsigned int num) {
    unsigned int reversed = 0;
    for (int i = 0; i < sizeof(num) * 8; i++) {
        reversed <<= 1;          // Shift reversed to left
        reversed |= (num & 1);  // Add last bit of num to reversed
        num >>= 1;              // Shift num to right
    }
    return reversed;
}

// Problem: Check if a number is a power of two.

int isPowerOfTwo(int num) {
    return num > 0 && (num & (num - 1)) == 0; // A power of two has only one set bit
}

// Problem: Count the number of bits required to convert integer A to integer B.

int countBitsToConvert(int a, int b) {
    int count = 0;
    int xor = a ^ b; // XOR will give bits that are different
    while (xor) {
        count += xor & 1; // Increment count if last bit is 1
        xor >>= 1;        // Right shift to check next bit
    }
    return count;
}

// Problem: Find the only non-repeating element in an array where every other element

int findNonRepeatingElement(int arr[], int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= arr[i]; // XORing all elements will cancel out repeating elements
    }
    return result;
}

// Problem: Generate all subsets of a set using bit manipulation.

void generateSubsets(int set[], int size) {
    int totalSubsets = 1 << size; // 2^size subsets
    for (int i = 0; i < totalSubsets; i++) {
        printf("{ ");
        for (int j = 0; j < size; j++) {
            if (i & (1 << j)) { // Check if jth bit is set
                printf("%d ", set[j]);
            }
        }
        printf("}\n");
    }
}

// Problem: Find the missing number in an array of size n-1 containing numbers

int findMissingNumber(int arr[], int size) {
    int n = size + 1; // Since one number is missing
    int totalXOR = 0;
    for (int i = 1; i <= n; i++) {
        totalXOR ^= i; // XOR of all numbers from 1 to n
    }
    int arrXOR = 0;
    for (int i = 0; i < size; i++) {
        arrXOR ^= arr[i]; // XOR of all elements in the array
    }
    return totalXOR ^ arrXOR; // Missing number is the XOR of the two results
}

// Problem: Count the number of bits to flip to convert A to B.

int countBitsToFlip(int a, int b) {
    int xor = a ^ b;
    int count = 0;
    while (xor) {
        count += xor & 1;
        xor >>= 1;
    }
    return count;
}

// Problem: Check if a number is a palindrome in binary representation.

int isBinaryPalindrome(int num) {
    int reversed = 0, original = num;
    while (num > 0) {
        reversed = (reversed << 1) | (num & 1);
        num >>= 1;
    }
    return original == reversed;
}

// Problem: Perform addition of two integers without using the '+' operator.

int addIntegers(int a, int b) {
    while (b != 0) {
        int carry = a & b; // Calculate carry
        a = a ^ b;         // Sum without carry
        b = carry << 1;    // Shift carry to the left
    }
    return a;
}

// Problem: Find the two non-repeating elements in an array where every other element

void findNonRepeatingElements(int arr[], int size) {
    int xorAll = 0;
    for (int i = 0; i < size; i++) {
        xorAll ^= arr[i]; // XOR of all elements
    }

    // Find a set bit (rightmost set bit)
    int setBit = xorAll & -xorAll;

    int num1 = 0, num2 = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] & setBit) {
            num1 ^= arr[i]; // Group with the set bit
        } else {
            num2 ^= arr[i]; // Group without the set bit
        }
    }

    printf("The two non-repeating elements are: %d and %d\n", num1, num2);  
}

