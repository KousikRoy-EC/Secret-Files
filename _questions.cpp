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

