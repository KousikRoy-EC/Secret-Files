#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

// 1. Prime no

/*
bool isPrime(int num){
    if(num<=1){
        return false;
    }

    for (int i = 2; i <= sqrt(num); i++){
       if(num%i==0){
        return false;
       }
    }
    return true;
}

int main(){
    int num;
    printf("Enter no to check Prime or not : ");
    scanf("%d",&num);
    isPrime(num)?printf("Prime"):printf("Not Prime");
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


// using recursion


void fibonacci(int num,int num1,int num2){

    if(num==0){
        return;
    }
    int sum=num1+num2;
    cout<<sum<<" ";
    fibonacci(num-1,num2,sum);
}
int main(){
    int num;
    cout<<"Enter no to print fibonacci series";
    cin>>num;
    cout<<"0 1 ";
    fibonacci(num-2,0,1);
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
    cout<<ptr(2,3); // Calling A Function Through Function Pointer (without parenthesis) // both are same
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
0 1 2 3 4
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


/* third largest element 

int thirdLargestElement(int *array,int size){
    int firstLargestVar=INT_MIN,secondLargestVar=INT_MIN,thirdLargestVar=INT_MIN;
    
    for(int i=0;i<size;i++){
        if(firstLargestVar<array[i]){
            thirdLargestVar=secondLargestVar;
            secondLargestVar=firstLargestVar;
            firstLargestVar=array[i];
        }
        else if(secondLargestVar<array[i] && array[i]!=firstLargestVar){
            thirdLargestVar=secondLargestVar;
            secondLargestVar=array[i];
        }
        else if(thirdLargestVar<array[i] && array[i]!=secondLargestVar && array[i]!=firstLargestVar){
            thirdLargestVar=array[i];
        }
    }

    return thirdLargestVar;
}

int main(){
    int arr[]={999,953,5,6,7,23,543,87,23,543};
    int result = thirdLargestElement(arr,(sizeof(arr)/sizeof(arr[0])));
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
        int curr_max = 0;
        for (int j = i ; j < Arr.size(); j++)
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
        int mid = low + (high - low) / 2;
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


// Problem: Check if a number is a power of four.

int isPowerofFour(int num) {
    // O(1) bitwise approach: A power of 4 is a power of 2 with its single set bit at an even position (mask 0x55555555)
    return (num > 0) && ((num & (num - 1)) == 0) && ((num & 0x55555555) != 0);
}

// Problem: Count the number of bits required to convert integer A to integer B.

int countBitsToConvert(int a, int b) {
    int count = 0;
    int diff = a ^ b; // XOR will give bits that are different
    while (diff) {
        count += diff & 1; // Increment count if last bit is 1
        diff >>= 1;        // Right shift to check next bit
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
    int diff = a ^ b;
    int count = 0;
    while (diff) {
        count += diff & 1;
        diff >>= 1;
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

// Problem multiply two numbers using bit manipulation

int multiply(int x, int y) {
    int result = 0;
    while (y != 0) {
        if (y & 1)          // if lowest bit of y is 1
            result += x;    // add x into result
        x <<= 1;            // x = x * 2 (shift left)
        y >>= 1;            // y = y / 2 (shift right)
    }
    return result;
}

// Problem: Find the two non-repeating elements in an array where every other element repeat twice

void findNonRepeatingElements(int arr[], int size) {
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
  5 ) Remove Duplicate Element from unsorted array 


#include <stdio.h>

int removeDuplicate(int *arr,int size){
    int hashTable[1024];
    int newSize=0;

    for(int idx=0;idx<size;idx++){
        hashTable[arr[idx]]++;
    }

    for(int idx=0;idx<=1024;idx++){
        if(hashTable[idx] > 0){
            arr[newSize++]=idx;
            hashTable[idx]=0;
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

// Method 1: Naive O(n*d) rotation
void leftRotateByD_naive(int *arr,int size,int d){
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

// Method 2: Optimal O(n) reversal algorithm
void leftRotateByD(int *arr,int size,int d){
    d = d % size;
    reverseArray(arr, d);
    reverseArray(arr + d, size - d);
    reverseArray(arr, size);
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


/* 18 ) Majority Element 
Algorithm (Boyer Moore Voting Algorithm) 
it states that if we have a majority element in an array then it will be the at last of the  array if not there then the array will not have a majority element only 

Example : [2,2,1,1,1,2,2] -> 2 is the majority element
Example : [1,2,3,4,5] -> no majority element

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

struct Node* removeDuplicates(struct Node* head) {
    struct Node* curr1 = head; 

    // Traverse each node in the list
    while (curr1 != NULL) {
        struct Node* curr2 = curr1; 

        // Traverse the remaining nodes to find and 
        // remove duplicates
        while (curr2->next != NULL) {
            
            // Check if the next node has the same 
            // data as the current node
            if (curr2->next->data == curr1->data) {
                
                // Duplicate found, remove it
                struct Node* duplicate = curr2->next;  
                curr2->next = curr2->next->next;  

                // Free the memory of the duplicate node
                free(duplicate);
            } else {
              
                // If the next node has different data from 
                // the current node, move to the next node
                curr2 = curr2->next;
            }
        }
        
        // Move to the next node in the list
        curr1 = curr1->next;
    }
    return head;
}



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




/* Intersection of Two Linked Lists (Helper approach)
typedef struct Node{
    int data;
    struct Node* next;
}Node;

Node* createNode(int val){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = val;
    newNode->next = NULL;
    return newNode;
}

Node* helperIntersection(Node* ListNode1, Node* ListNode2, int l1, int l2){
    int count=l1-l2;
    while(count--) ListNode1 = ListNode1->next;
    
    while(ListNode1 && ListNode2) {
        if(ListNode1 == ListNode2) return ListNode1;
        
        ListNode1 = ListNode1->next;
        ListNode2 = ListNode2->next;
    }
    return NULL;
}

int findLength(Node* ListNode){
    int count = 0;
    while(ListNode){
        count++;
        ListNode = ListNode->next;
    }
    return count;
}
    

Node* findIntersection(Node* ListNode1, Node* ListNode2){
    if(ListNode1 == NULL || ListNode2 == NULL) return NULL;
    
    int Length1 = findLength(ListNode1);
    int Length2 = findLength(ListNode2);
    
    Node* resultNode;
    
    if(Length1 < Length2){
        resultNode = helperIntersection(ListNode2,ListNode1,Length2,Length1);
    }
    else{
        resultNode = helperIntersection(ListNode1,ListNode2,Length1, Length2);
    }
    return resultNode;
}

int main()
{
    Node *common1 = createNode(30);
    Node *common2 = createNode(40);
    Node *common3 = createNode(50);

    common1->next = common2;
    common2->next = common3;

    Node *head1 = createNode(10);
    Node *node20 = createNode(20);

    head1->next = node20;
    node20->next = common1;

    Node *head2 = createNode(15);
    Node *node25 = createNode(25);

    head2->next = node25;
    node25->next = common1;
   
    Node *intersection = findIntersection(head1, head2);

    if (intersection != NULL) {
        printf("Intersection node = %d\n", intersection->data);
    }
    else {
        printf("No intersection\n");
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

void deleteNode(struct Node **head, struct Node *nodeToDelete)
{
    if (*head == NULL || nodeToDelete == NULL)
        return;

    if (*head == nodeToDelete)
    {
        *head = nodeToDelete->next;
        free(nodeToDelete);
        return;
    }

    struct Node *temp = *head;

    while (temp != NULL && temp->next != nodeToDelete)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        return;  // node not found
    }

    temp->next = nodeToDelete->next;
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
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* reverse(struct Node* head) {
    struct Node* prev = NULL;
    struct Node* curr = head;
    struct Node* next;

    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

int isIdentical(struct Node* n1, struct Node* n2) {
    for (; n1 && n2; n1 = n1->next, n2 = n2->next)
        if (n1->data != n2->data)
            return 0;

    return 1;
}

int isPalindrome(struct Node* head) {
    if (!head || !head->next)
        return 1;

    struct Node *slow = head, *fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    struct Node* head2 = reverse(slow->next);
    slow->next = NULL;

    int ret = isIdentical(head, head2);

    head2 = reverse(head2);
    slow->next = head2;

    return ret;
}

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

int main() {
  	
  	// Linked list : 1->2->3->2->1
    struct Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(2);
    head->next->next->next->next = createNode(1);

    int result = isPalindrome(head);

    if (result)
        printf("true\n");
    else
        printf("false\n");

    return 0;
}


/* 10 ) Rotate a Linked List

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;

Node* createNode(int val){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = val;
    newNode->next = NULL;
    return newNode;
}

int findLength(Node* head){
    int count = 0;
    Node* temp = head;
    
    while(temp != NULL){
        count++;
        temp=temp->next;
    }
    return count;
}       

Node* rotateLinkedList(Node** ListNode,int pos){
    if(*ListNode == NULL) return NULL;
    int length = findLength(*ListNode);
    pos = pos%length;
    if(pos == 0) return *ListNode;
    
    Node* list = *ListNode;
    int count = 1;
    
    while(count++ < pos) list = list->next;
    
    Node *tempTail = list;
    
    while(list->next != NULL) list = list->next;
    
    list->next = *ListNode;
    
    list = tempTail->next;
    tempTail->next = NULL;
    return list;
}

void printList(Node* ListNode){
    while(ListNode != NULL){
        printf("%d->",ListNode->data);
        ListNode = ListNode->next;
    }
}

int main()
{
    Node *common1 = createNode(30);
    Node *common2 = createNode(40);
    Node *common3 = createNode(50);
    common1->next = common2;
    common2->next = common3;
    Node *head1 = createNode(10);
    Node *node20 = createNode(20);
    head1->next = node20;
    node20->next = common1;
    int position = 3;

    Node* result = rotateLinkedList(&head1, position);
    printList(result);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

void rotateLinkedList(struct Node *head,int k){
    if(head==NULL || k==0){
        return;
    }

    struct Node *current=head;
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
    current->next=head;
    head=kthNode->next;
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



// Reverse a string in-place without a second buffer.

/*
#include <stdio.h>
#include <string.h>

void reverseString(char* str) {
    int n = strlen(str);
    char* start = str;
    char* end = str + n - 1;

    while (start < end) {
        // Swap characters
        char temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}

int main() {
    char str[] = "hello";
    reverseString(str);
    printf("Reversed string: %s\n", str); // Output: olleh
    return 0;
}
*/

// Check if a string is a palindrome using two pointers.

/*
#include <stdio.h>
#include <string.h>

int isPalindrome(char* str) {
    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        if (str[left] != str[right]) {
            return 0; // Not a palindrome
        }
        left++;
        right--;
    }
    return 1; // Is a palindrome
}

int main() {
    char str1[] = "madam";
    char str2[] = "hello";

    printf("%s is palindrome: %d\n", str1, isPalindrome(str1)); // Output: 1
    printf("%s is palindrome: %d\n", str2, isPalindrome(str2)); // Output: 0

    return 0;
}
*/

// Remove duplicate characters from a string in-place.

/*
#include <stdio.h>
#include <string.h>

void removeDuplicates(char* str) {
    int n = strlen(str);
    if (n == 0) return;

    // Sort the string (duplicates will be adjacent)
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (str[i] > str[j]) {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }

    // Compact the array
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 || str[i] != str[i - 1]) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0'; // Null-terminate
}

int main() {
    char str[] = "programming";
    removeDuplicates(str);
    printf("String after removing duplicates: %s\n", str); // Output: agimmnoprr

    return 0;
}
*/

// Find the first non-repeating character in a string.

/*
#include <stdio.h>
#include <string.h>

char firstNonRepeating(char* str) {
    int count[256] = {0}; // Assuming ASCII characters

    // First pass: count frequencies
    for (int i = 0; str[i] != '\0'; i++) {
        count[(unsigned char)str[i]]++;
    }

    // Second pass: find the first character with count 1
    for (int i = 0; str[i] != '\0'; i++) {
        if (count[(unsigned char)str[i]] == 1) {
            return str[i];
        }
    }

    return '\0'; // No non-repeating character found
}

int main() {
    char str[] = "leetcode";
    char result = firstNonRepeating(str);

    if (result != '\0') {
        printf("First non-repeating character: %c\n", result); // Output: l
    } else {
        printf("No non-repeating character found\n");
    }

    return 0;
}
*/  




/* 2. Binary to Decimal Conversion

#include <stdio.h>

int binaryToDecimal(long long n) {
    int decimalNumber = 0, i = 0, remainder;

    while (n != 0) {
        remainder = n % 10;
        n /= 10;
        decimalNumber += remainder * (1 << i); // 1 << i is equivalent to 2^i
        i++;
    }
    return decimalNumber;
}

int main() {
    long long binary = 11011; // 27 in decimal
    printf("Binary %lld in Decimal = %d\n", binary, binaryToDecimal(binary));
    return 0;
}
*/

/* Decimal to Binary (Supporting Negative Numbers)

#include <stdio.h>

void decimalToBinary(int n) {
    // Determine the number of bits in an integer (typically 32 bits)
    int bits = sizeof(int) * 8;
    int flag = 0; // Used to drop leading zeros for cleaner output

    printf("Decimal %d in Binary = ", n);

    // Loop through all bits from Most Significant Bit (MSB) to Least Significant Bit (LSB)
    for (int i = bits - 1; i >= 0; i--) {
        int bit = ((unsigned int)n >> i) & 1;

        if (bit == 1) {
            flag = 1; // Found the first '1', start printing from here
        }

        if (flag) {
            printf("%d", bit);
        }
    }
    
    // If the number was 0, the loop wouldn't print anything
    if (flag == 0) {
        printf("0");
    }
    printf("\n");
}

int main() {
    int positiveNum = 27;
    int negativeNum = -27;

    decimalToBinary(positiveNum);
    decimalToBinary(negativeNum); // Prints the 2's complement representation

    return 0;
}
*/

/* Endianness Conversion Using a UNION

#include <stdio.h>
#include <stdint.h>

// Define a union where a 32-bit int and a 4-byte array overlap in memory
union EndianConverter {
    uint32_t value;
    uint8_t bytes[4];
};

uint32_t switchEndianness(uint32_t num) {
    union EndianConverter source;
    union EndianConverter target;

    source.value = num;

    // Manually reverse the byte order
    target.bytes[0] = source.bytes[3];
    target.bytes[1] = source.bytes[2];
    target.bytes[2] = source.bytes[1];
    target.bytes[3] = source.bytes[0];

    return target.value;
}

int main() {
    // Example value: 0x12345678
    // In Little-Endian (x86/x64), it sits in memory as: [78, 56, 34, 12]
    uint32_t littleEndian = 0x12345678;
    uint32_t bigEndian = switchEndianness(littleEndian);

    printf("Original (Little-Endian): 0x%X\n", littleEndian);
    printf("Converted (Big-Endian):    0x%X\n", bigEndian);

    return 0;
}
*/






#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ---------------------------------------------------------
   1. Reverse an array in-place            [Easy] O(n)/O(1)
   --------------------------------------------------------- */
void reverseArray(int arr[], int n) {
    int l = 0, r = n - 1;
    while (l < r) {
        int t = arr[l];
        arr[l] = arr[r];
        arr[r] = t;
        l++; r--;
    }
}

/* ---------------------------------------------------------
   2. Rotate an array by K positions       [Medium] O(n)/O(1)
   Idea: Reversal algorithm.
   Left-rotate by k:  reverse(0,k-1); reverse(k,n-1); reverse(0,n-1)
   --------------------------------------------------------- */
static void reverseRange(int arr[], int l, int r) {
    while (l < r) { int t = arr[l]; arr[l] = arr[r]; arr[r] = t; l++; r--; }
}
void rotateArrayLeft(int arr[], int n, int k) {
    k %= n;
    if (k == 0) return;
    reverseRange(arr, 0, k - 1);
    reverseRange(arr, k, n - 1);
    reverseRange(arr, 0, n - 1);
}
/* To rotate RIGHT by k, just call rotateArrayLeft(arr, n, n - k % n). */

/* ---------------------------------------------------------
   3. Find the second largest element      [Easy] O(n)/O(1)
   Single pass, track largest & second largest.
   --------------------------------------------------------- */
int secondLargest(int arr[], int n) {
    if (n < 2) { fprintf(stderr, "Need >=2 elements\n"); return INT_MIN; }
    int first = INT_MIN, second = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (arr[i] > first) {
            second = first;
            first = arr[i];
        } else if (arr[i] > second && arr[i] != first) {
            second = arr[i];
        }
    }
    return second; /* INT_MIN if no valid second-largest (all equal) */
}

/* ---------------------------------------------------------
   4. Remove duplicates from a SORTED array [Easy] O(n)/O(1)
   Two-pointer (write index) technique. Returns new length.
   --------------------------------------------------------- */
int removeDuplicatesSorted(int arr[], int n) {
    if (n == 0) return 0;
    int writeIdx = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] != arr[writeIdx]) {
            arr[++writeIdx] = arr[i];
        }
    }
    return writeIdx+1; // new Size
}

/* ---------------------------------------------------------
   5. Move all zeros to the end, keep order [Easy] O(n)/O(1)
   Two-pointer: writeIdx tracks next non-zero slot.
   --------------------------------------------------------- */

void moveZerosToEnd(int arr[], int n) {
    int writeIdx = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i] != 0) {
            int temp = arr[i];
            arr[i] = arr[writeIdx];
            arr[writeIdx] = temp;
            writeIdx++;
        }
    }
}

void findDuplicates_inplace(int arr[], int n) {
    printf("Duplicates: ");
    for (int i = 0; i < n; i++) {
        int idx = abs(arr[i]);
        if (arr[idx] < 0) {
            printf("%d ", idx);
        } else {
            arr[idx] = -arr[idx];
        }
    }
    printf("\n");
    /* restore array if needed by taking abs of every element */
    for (int i = 0; i < n; i++) arr[i] = abs(arr[i]);
}


/* ---------------------------------------------------------
   14. Intersection of two SORTED arrays    [Medium] O(n+m)/O(1) extra
   Two-pointer merge-style walk.
   --------------------------------------------------------- */
void intersectionSorted(int a[], int n, int b[], int m) {
    int i = 0, j = 0;
    printf("Intersection: ");
    while (i < n && j < m) {
        if (a[i] < b[j]) i++;
        else if (a[i] > b[j]) j++;
        else {
            printf("%d ", a[i]);
            i++; j++;
            /* skip duplicates if arrays may contain repeats and
               you want unique intersection values: */
            while (i < n && a[i] == a[i-1]) i++;
        }
    }
    printf("\n");
}


/* ---- Queue (FIFO) — circular array implementation ---- */
#define CAP 100


typedef struct {
    int data[CAP];
    int front, rear, count;
} Queue;

void queueInit(Queue *q) { q->front = 0; q->rear = -1; q->count = 0; }
int  queueIsEmpty(Queue *q) { return q->count == 0; }
int  queueIsFull(Queue *q)  { return q->count == CAP; }
void enqueue(Queue *q, int val) {
    if (queueIsFull(q)) { printf("Queue overflow\n"); return; }
    q->rear = (q->rear + 1) % CAP;
    q->data[q->rear] = val;
    q->count++;
}
int dequeue(Queue *q) {
    if (queueIsEmpty(q)) { printf("Queue underflow\n"); return -1; }
    int val = q->data[q->front];
    q->front = (q->front + 1) % CAP;
    q->count--;
    return val;
}





/* ============================================================
   LINKED LIST — INTERVIEW QUESTIONS (C IMPLEMENTATIONS)
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* newNode(int val) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->data = val;
    return n;
}

/* ---------------------------------------------------------
   10. Detect and remove a loop            [Hard]
   Floyd's cycle detection (slow/fast pointers), then find
   the loop's start and break it. O(n)/O(1).
   --------------------------------------------------------- */
void detectAndRemoveLoop(Node *head) {
    if (!head || !head->next) return;

    Node *slow = head, *fast = head;
    int loopFound = 0;

    /* Step 1: detect using Floyd's algorithm */
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) { loopFound = 1; break; }
    }
    if (!loopFound) return; /* no loop */

    /* Step 2: find start of loop.
       Move slow back to head; move both one step at a time;
       they meet at the loop's starting node. */
    slow = head;
    if (slow == fast) {
        /* loop starts at head — advance fast till it's one node
           before head again to find the tail that points back */
        while (fast->next != slow) fast = fast->next;
    } else {
        while (slow->next != fast->next) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    /* fast->next is the last node in the loop; break it */
    fast->next = NULL;
}

/* ---------------------------------------------------------
   14. Insert into a sorted linked list    [Easy] O(n)/O(1)
   --------------------------------------------------------- */
Node* insertSorted(Node *head, int val) {
    Node *newN = newNode(val);
    if (!head || head->data >= val) {
        newN->next = head;
        return newN;
    }
    Node *curr = head;
    while (curr->next && curr->next->data < val)
        curr = curr->next;
    newN->next = curr->next;
    curr->next = newN;
    return head;
}

/* ---------------------------------------------------------
   15. Implement a circular linked list    [Medium]
   Last node points back to head instead of NULL.
   --------------------------------------------------------- */
typedef struct {
    Node *head;
    Node *tail; /* tail->next == head always, for O(1) insert-at-end */
} CircularList;

void circularInit(CircularList *list) {
    list->head = list->tail = NULL;
}

void circularInsertEnd(CircularList *list, int val) {
    Node *n = newNode(val);
    if (!list->head) {
        list->head = list->tail = n;
        n->next = n; /* points to itself */
        return;
    }
    n->next = list->head;
    list->tail->next = n;
    list->tail = n;
}


void circularInsertFront(CircularList* list, int val) {
    Node *n = newNode(val);
    if (!list->head) {
        list->head = list->tail = n;
        n->next = n;
        return;
    }
    n->next = list->head;
    list->tail->next = n;
    list->head = n;
}


void circularPrint(CircularList *list) {
    if (!list->head) { printf("(empty)\n"); return; }
    Node *curr = list->head;
    do {
        printf("%d -> ", curr->data);
        curr = curr->next;
    } while (curr != list->head);
    printf("(head)\n");
}

/* delete a node by value from circular list */
void circularDelete(CircularList *list, int val) {
    if (!list->head) return;

    Node *curr = list->head, *prev = list->tail;
    do {
        if (curr->data == val) {
            if (curr == list->head && curr == list->tail) {
                list->head = list->tail = NULL;
            } else {
                prev->next = curr->next;
                if (curr == list->head) list->head = curr->next;
                if (curr == list->tail) list->tail = prev;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != list->head);
}







/* ============================================================
   BINARY TREE — INTERVIEW QUESTIONS (C IMPLEMENTATIONS)
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXN 100

typedef struct TreeNode {
    int data;
    struct TreeNode *left, *right;
} TreeNode;

TreeNode* newTreeNode(int val) {
    TreeNode *n = (TreeNode*)malloc(sizeof(TreeNode));
    n->data = val;
    n->left = n->right = NULL;
    return n;
}

/* ---------------------------------------------------------
   1. Inorder traversal — Recursive & Iterative [Easy]
   Recursive: Left, Root, Right
   Iterative: explicit stack simulates recursion
   --------------------------------------------------------- */
void inorderRecursive(TreeNode *root) {
    if (!root) return;
    inorderRecursive(root->left);
    printf("%d ", root->data);
    inorderRecursive(root->right);
}

void inorderIterative(TreeNode *root) {
    TreeNode *stack[MAXN];
    int top = -1;
    TreeNode *curr = root;
    while (curr || top != -1) {
        while (curr) {
            stack[++top] = curr;
            curr = curr->left;
        }
        curr = stack[top--];
        printf("%d ", curr->data);
        curr = curr->right;
    }
}

/* ---------------------------------------------------------
   2. Preorder traversal                    [Easy]
   Root, Left, Right
   --------------------------------------------------------- */
void preorderRecursive(TreeNode *root) {
    if (!root) return;
    printf("%d ", root->data);
    preorderRecursive(root->left);
    preorderRecursive(root->right);
}

void preorderIterative(TreeNode *root) {
    if (!root) return;
    TreeNode *stack[MAXN];
    int top = -1;
    stack[++top] = root;
    while (top != -1) {
        TreeNode *node = stack[top--];
        printf("%d ", node->data);
        if (node->right) stack[++top] = node->right;
        if (node->left)  stack[++top] = node->left;
    }
}

/* ---------------------------------------------------------
   3. Postorder traversal                   [Medium]
   Left, Right, Root — trickiest to do iteratively.
   Two-stack method shown (easy to reason about).
   --------------------------------------------------------- */
void postorderRecursive(TreeNode *root) {
    if (!root) return;
    postorderRecursive(root->left);
    postorderRecursive(root->right);
    printf("%d ", root->data);
}

void postorderIterative(TreeNode *root) {
    if (!root) return;
    TreeNode *s1[MAXN], *s2[MAXN];
    int top1 = -1, top2 = -1;
    s1[++top1] = root;
    while (top1 != -1) {
        TreeNode *node = s1[top1--];
        s2[++top2] = node;
        if (node->left)  s1[++top1] = node->left;
        if (node->right) s1[++top1] = node->right;
    }
    while (top2 != -1) printf("%d ", s2[top2--]->data);
}

/* ---------------------------------------------------------
   4. Level order traversal (BFS)           [Medium] O(n)
   Uses a simple circular-array queue.
   --------------------------------------------------------- */
void levelOrder(TreeNode *root) {
    if (!root) return;
    TreeNode *queue[MAXN];
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        TreeNode *node = queue[front++];
        printf("%d ", node->data);
        if (node->left)  queue[rear++] = node->left;
        if (node->right) queue[rear++] = node->right;
    }
}
/* Level-by-level (with newline per level) */
void levelOrderByLevel(TreeNode *root) {
    if (!root) return;
    TreeNode *queue[MAXN];
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        int levelSize = rear - front;
        for (int i = 0; i < levelSize; i++) {
            TreeNode *node = queue[front++];
            printf("%d ", node->data);
            if (node->left)  queue[rear++] = node->left;
            if (node->right) queue[rear++] = node->right;
        }
        printf("\n");
    }
}

/* ---------------------------------------------------------
   5. Height of a binary tree               [Easy] O(n)
   --------------------------------------------------------- */
int treeHeight(TreeNode *root) {
    if (!root) return 0;
    int lh = treeHeight(root->left);
    int rh = treeHeight(root->right);
    return 1 + (lh > rh ? lh : rh);
}

/* ---------------------------------------------------------
   6. Check if a binary tree is balanced    [Medium]
   Balanced: |height(left) - height(right)| <= 1 for every node.
   Efficient O(n) approach: compute height + check in one pass,
   returning -1 as a sentinel for "already unbalanced".
   --------------------------------------------------------- */
int isBalancedHelper(TreeNode *root, int *isBalanced) {
    if (!root) return 0;
    int lh = isBalancedHelper(root->left, isBalanced);
    int rh = isBalancedHelper(root->right, isBalanced);
    if (abs(lh - rh) > 1) *isBalanced = 0;
    return 1 + (lh > rh ? lh : rh);
}
int isBalanced(TreeNode *root) {
    int balanced = 1;
    isBalancedHelper(root, &balanced);
    return balanced;
}

/* ---------------------------------------------------------
   7. Diameter of a binary tree             [Medium]
   Diameter = longest path between any two nodes (in edges).
   O(n) single-pass approach, tracking max diameter globally.
   --------------------------------------------------------- */
int diameterHelper(TreeNode *root, int *diameter) {
    if (!root) return 0;
    int lh = diameterHelper(root->left, diameter);
    int rh = diameterHelper(root->right, diameter);
    if (lh + rh > *diameter) *diameter = lh + rh;
    return 1 + (lh > rh ? lh : rh);
}
int diameter(TreeNode *root) {
    int d = 0;
    diameterHelper(root, &d);
    return d;
}

/* ---------------------------------------------------------
   8. Lowest Common Ancestor (LCA)          [Medium] O(n)
   Works for a general binary tree (not necessarily BST).
   --------------------------------------------------------- */
TreeNode* findLCA(TreeNode *root, int n1, int n2) {
    if (!root) return NULL;
    if (root->data == n1 || root->data == n2) return root;

    TreeNode *left = findLCA(root->left, n1, n2);
    TreeNode *right = findLCA(root->right, n1, n2);

    if (left && right) return root; /* n1 in one subtree, n2 in other */
    return left ? left : right;
}
/* For a BST specifically, this can be done in O(h) without recursion
   into both subtrees — using BST ordering property:
TreeNode* findLCA_BST(TreeNode *root, int n1, int n2) {
    while (root) {
        if (n1 < root->data && n2 < root->data) root = root->left;
        else if (n1 > root->data && n2 > root->data) root = root->right;
        else return root;
    }
    return NULL;
}
*/

/* ---------------------------------------------------------
   9. Check whether two trees are identical [Easy] O(n)
   --------------------------------------------------------- */
int areIdentical(TreeNode *a, TreeNode *b) {
    if (!a && !b) return 1;
    if (!a || !b) return 0;
    return (a->data == b->data)
        && areIdentical(a->left, b->left)
        && areIdentical(a->right, b->right);
}

/* ---------------------------------------------------------
   11. Validate whether a tree is a BST     [Medium] O(n)
   Approach: pass down valid (min, max) range for each node.
   --------------------------------------------------------- */
int isBSTHelper(TreeNode *root, long minVal, long maxVal) {
    if (!root) return 1;
    if (root->data <= minVal || root->data >= maxVal) return 0;
    return isBSTHelper(root->left, minVal, root->data)
        && isBSTHelper(root->right, root->data, maxVal);
}
int isBST(TreeNode *root) {
    return isBSTHelper(root, LONG_MIN, LONG_MAX);
}

/* ---------------------------------------------------------
   12. Print left view and right view       [Medium] O(n)
   BFS level order; first node per level = left view,
   last node per level = right view.
   --------------------------------------------------------- */
void printLeftView(TreeNode *root) {
    if (!root) return;
    TreeNode *queue[MAXN];
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        int levelSize = rear - front;
        for (int i = 0; i < levelSize; i++) {
            TreeNode *node = queue[front++];
            if (i == 0) printf("%d ", node->data); /* first in level */
            if (node->left)  queue[rear++] = node->left;
            if (node->right) queue[rear++] = node->right;
        }
    }
}
void printRightView(TreeNode *root) {
    if (!root) return;
    TreeNode *queue[MAXN];
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        int levelSize = rear - front;
        for (int i = 0; i < levelSize; i++) {
            TreeNode *node = queue[front++];
            if (i == levelSize - 1) printf("%d ", node->data); /* last in level */
            if (node->left)  queue[rear++] = node->left;
            if (node->right) queue[rear++] = node->right;
        }
    }
}



/* ---------------------------------------------------------
   13. Print top view and bottom view       [Hard]
   Approach: assign each node a horizontal distance (HD) from
   root (left: -1, right: +1). BFS; for TOP view keep the
   FIRST node seen at each HD; for BOTTOM view keep the LAST.
   --------------------------------------------------------- */
typedef struct { TreeNode *node; int hd; } HDNode;
 
void printTopView(TreeNode *root) {
    if (!root) return;
    int minHD = 0, maxHD = 0, seen[2 * MAXN] = {0}, val[2 * MAXN];
    HDNode queue[MAXN];
    int front = 0, rear = 0;
    queue[rear++] = (HDNode){root, 0};
 
    while (front < rear) {
        HDNode curr = queue[front++];
        int idx = curr.hd + MAXN; /* offset to avoid negative index */
        if (!seen[idx]) {
            seen[idx] = 1;
            val[idx] = curr.node->data;
            if (curr.hd < minHD) minHD = curr.hd;
            if (curr.hd > maxHD) maxHD = curr.hd;
        }
        if (curr.node->left)
            queue[rear++] = (HDNode){curr.node->left, curr.hd - 1};
        if (curr.node->right)
            queue[rear++] = (HDNode){curr.node->right, curr.hd + 1};
    }
    for (int hd = minHD; hd <= maxHD; hd++)
        printf("%d ", val[hd + MAXN]);
    printf("\n");
}
 
void printBottomView(TreeNode *root) {
    if (!root) return;
    int minHD = 0, maxHD = 0, seen[2 * MAXN] = {0}, val[2 * MAXN];
    HDNode queue[MAXN];
    int front = 0, rear = 0;
    queue[rear++] = (HDNode){root, 0};
 
    while (front < rear) {
        HDNode curr = queue[front++];
        int idx = curr.hd + MAXN;
        seen[idx] = 1;
        val[idx] = curr.node->data; /* overwrite -> last node at this HD wins */
        if (curr.hd < minHD) minHD = curr.hd;
        if (curr.hd > maxHD) maxHD = curr.hd;
        if (curr.node->left)
            queue[rear++] = (HDNode){curr.node->left, curr.hd - 1};
        if (curr.node->right)
            queue[rear++] = (HDNode){curr.node->right, curr.hd + 1};
    }
    for (int hd = minHD; hd <= maxHD; hd++)
        printf("%d ", val[hd + MAXN]);
    printf("\n");
}
 


/* ============================================================================
   TOP 50 C CODING INTERVIEW QUESTIONS (3 YEARS OF EXPERIENCE LEVEL)
   Target Roles: Systems Engineer, Software Engineer (C / Low-Level), Product Companies
   ============================================================================ */

/* ============================================================================
   SECTION 1: STRING MANIPULATION WITHOUT STANDARD LIBRARY HELPERS (Q1 - Q10)
   ============================================================================ */

/* ----------------------------------------------------------------------------
   Q1. Implement Custom strstr (Substring Search / Needle in Haystack)
   Difficulty: Medium | Time: O(N * M), Space: O(1)
   Why Asked: Tests nested pointer navigation, early exit conditions, and
              avoidance of standard library functions.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

char* my_strstr(const char *haystack, const char *needle) {
    if (!haystack || !needle) return NULL;
    if (*needle == '\0') return (char*)haystack; // Empty needle matches start

    while (*haystack != '\0') {
        if (*haystack == *needle) {
            const char *h = haystack;
            const char *n = needle;

            while (*h != '\0' && *n != '\0' && *h == *n) {
                h++;
                n++;
            }
            if (*n == '\0') {
                return (char*)haystack; // Full needle matched
            }
        }
        haystack++;
    }
    return NULL;
}

int main() {
    const char *text = "Product Based Company Interview";
    const char *sub = "Company";
    char *res = my_strstr(text, sub);
    if (res) printf("Found: %s\n", res);
    else printf("Not Found\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q2. Reverse Words in a Given Sentence In-Place
   Difficulty: Medium | Time: O(N), Space: O(1)
   Example: "the sky is blue" -> "blue is sky the"
   Why Asked: Tests two-pointer in-place string manipulation and word boundary
              tokenization without auxiliary buffers.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <string.h>

static void reverse_range(char *start, char *end) {
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void reverseWordsInSentence(char *str) {
    if (!str || !*str) return;

    int len = strlen(str);
    // Step 1: Reverse entire string
    reverse_range(str, str + len - 1);

    // Step 2: Reverse each individual word in place
    char *word_start = str;
    char *curr = str;

    while (*curr != '\0') {
        if (*curr == ' ') {
            reverse_range(word_start, curr - 1);
            word_start = curr + 1;
        }
        curr++;
    }
    // Reverse the last word
    reverse_range(word_start, curr - 1);
}

int main() {
    char s[] = "the sky is blue";
    printf("Original: \"%s\"\n", s);
    reverseWordsInSentence(s);
    printf("Reversed words: \"%s\"\n", s);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q3. Reentrant String Tokenizer (my_strtok_r)
   Difficulty: Medium-Hard | Time: O(N), Space: O(1)
   Why Asked: Tests understanding of static vs reentrant state (thread safety),
              delimiter matching, and pointer-to-pointer manipulation.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

char* my_strtok_r(char *str, const char *delim, char **saveptr) {
    char *token_start;

    if (!delim || !saveptr) return NULL;

    if (str == NULL) {
        str = *saveptr;
    }
    if (str == NULL) return NULL;

    // Helper lambda-like check for delimiter
    #define IS_DELIM(c) ({ \
        int _is = 0; \
        const char *_d = delim; \
        while (*_d) { if (*_d == (c)) { _is = 1; break; } _d++; } \
        _is; \
    })

    // Skip leading delimiters
    while (*str && IS_DELIM(*str)) {
        str++;
    }
    if (*str == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    token_start = str;

    // Find end of token
    while (*str && !IS_DELIM(*str)) {
        str++;
    }

    if (*str != '\0') {
        *str = '\0';
        *saveptr = str + 1;
    } else {
        *saveptr = NULL;
    }

    return token_start;
}

int main() {
    char text[] = "embedded,software,,developer;c-programming";
    const char *delims = ",;-";
    char *save = NULL;
    char *tok = my_strtok_r(text, delims, &save);
    while (tok) {
        printf("Token: %s\n", tok);
        tok = my_strtok_r(NULL, delims, &save);
    }
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q4. Run-Length Encoding (String Compression)
   Difficulty: Easy-Medium | Time: O(N), Space: O(1) auxiliary
   Example: "aabcccccaaa" -> "a2b1c5a3"
   Why Asked: Tests buffer management, integer-to-string encoding on the fly,
              and checking whether compressed string is smaller than original.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <string.h>

void compressString(const char *src, char *dest, size_t dest_size) {
    if (!src || !dest || dest_size == 0) return;

    size_t write_idx = 0;
    int src_len = strlen(src);

    for (int i = 0; i < src_len; i++) {
        char ch = src[i];
        int count = 1;

        while (i + 1 < src_len && src[i + 1] == ch) {
            count++;
            i++;
        }

        // Format into dest: char followed by count
        int written = snprintf(dest + write_idx, dest_size - write_idx, "%c%d", ch, count);
        if (written < 0 || (size_t)written >= dest_size - write_idx) {
            break; // Buffer full
        }
        write_idx += written;
    }
}

int main() {
    const char *raw = "aabcccccaaa";
    char compressed[64];
    compressString(raw, compressed, sizeof(compressed));
    printf("Original: %s\nCompressed: %s\n", raw, compressed);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q5. Check if One String is a Rotation of Another String
   Difficulty: Easy-Medium | Time: O(N), Space: O(N)
   Example: "waterbottle" is a rotation of "erbottlewat"
   Why Asked: Classic interview problem verifying understanding of string
              concatenation (s1 + s1 contains s2) and pointer search.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isStringRotation(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if (len1 != len2 || len1 == 0) return 0;

    // Allocate memory for s1 concatenated with itself
    char *concat = (char*)malloc(2 * len1 + 1);
    if (!concat) return 0;

    strcpy(concat, s1);
    strcat(concat, s1);

    int result = (strstr(concat, s2) != NULL);
    free(concat);
    return result;
}

int main() {
    const char *s1 = "waterbottle";
    const char *s2 = "erbottlewat";
    printf("Is rotation: %d\n", isStringRotation(s1, s2)); // 1
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q6. Robust String to Integer (my_atoi) with Overflow Handling
   Difficulty: Medium | Time: O(N), Space: O(1)
   Why Asked: Every interviewer tests this for candidates with 2-4 YoE because
              it exposes whether they handle: leading spaces, signs (+/-),
              invalid chars, and INT_MAX (2147483647) / INT_MIN (-2147483648) overflow.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <limits.h>
#include <ctype.h>

int my_atoi(const char *str) {
    if (!str) return 0;

    // 1. Skip leading whitespaces
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') {
        str++;
    }

    // 2. Check optional sign
    int sign = 1;
    if (*str == '+' || *str == '-') {
        if (*str == '-') sign = -1;
        str++;
    }

    // 3. Process digits with overflow detection
    long long result = 0;
    while (*str >= '0' && *str <= '9') {
        int digit = *str - '0';

        // Check overflow before multiplication
        if (sign == 1) {
            if (result > (INT_MAX - digit) / 10) {
                return INT_MAX;
            }
        } else {
            if (-result < (INT_MIN + digit) / 10) {
                return INT_MIN;
            }
        }

        result = result * 10 + digit;
        str++;
    }

    return (int)(sign * result);
}

int main() {
    printf("Result 1: %d\n", my_atoi("   -42"));             // -42
    printf("Result 2: %d\n", my_atoi("4193 with words"));     // 4193
    printf("Result 3: %d\n", my_atoi("-91283472332"));        // INT_MIN
    printf("Result 4: %d\n", my_atoi("2147483648"));          // INT_MAX
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q7. Integer to String (my_itoa) with Arbitrary Radix (Base 2 - 16)
   Difficulty: Medium | Time: O(log_base N), Space: O(1)
   Why Asked: Tests modulo arithmetic, handling negative numbers, INT_MIN edge case
              where -INT_MIN overflows 32-bit signed int, and string reversal.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

char* my_itoa(int value, char *str, int base) {
    if (!str || base < 2 || base > 16) return NULL;

    char *ptr = str;
    unsigned int uval;
    int is_negative = 0;

    // Base 10 handles negative sign; other bases treat value as unsigned
    if (base == 10 && value < 0) {
        is_negative = 1;
        // Use unsigned cast to safely handle INT_MIN without overflow
        uval = (unsigned int)(-(long long)value);
    } else {
        uval = (unsigned int)value;
    }

    if (uval == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }

    const char digits[] = "0123456789ABCDEF";
    char *start = ptr;

    while (uval > 0) {
        int rem = uval % base;
        *ptr++ = digits[rem];
        uval /= base;
    }

    if (is_negative) {
        *ptr++ = '-';
    }
    *ptr = '\0';

    // Reverse the generated digits
    char *end = ptr - 1;
    while (start < end) {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }

    return str;
}

int main() {
    char buf[64];
    printf("Base 10: %s\n", my_itoa(-12345, buf, 10)); // -12345
    printf("Base 16: 0x%s\n", my_itoa(255, buf, 16));   // FF
    printf("Base 2:  %s\n", my_itoa(13, buf, 2));       // 1101
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q8. Longest Common Prefix Among an Array of Strings
   Difficulty: Easy-Medium | Time: O(S) where S is sum of all characters
   Why Asked: Tests 2D pointer traversal, boundary checking, and common prefix logic.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <string.h>

char* longestCommonPrefix(char **strs, int strsSize, char *buffer, int bufferSize) {
    if (!strs || strsSize <= 0 || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return buffer;
    }

    int idx = 0;
    while (idx < bufferSize - 1) {
        char current_char = strs[0][idx];
        if (current_char == '\0') break;

        // Check if all other strings have the same character at this position
        int match = 1;
        for (int i = 1; i < strsSize; i++) {
            if (strs[i][idx] != current_char) {
                match = 0;
                break;
            }
        }
        if (!match) break;

        buffer[idx] = current_char;
        idx++;
    }

    buffer[idx] = '\0';
    return buffer;
}

int main() {
    char *words[] = {"flower", "flow", "flight"};
    char result[32];
    printf("LCP: \"%s\"\n", longestCommonPrefix(words, 3, result, sizeof(result))); // "fl"
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q9. In-Place Removal of Consecutive Spaces and Trimming
   Difficulty: Medium | Time: O(N), Space: O(1)
   Example: "   Hello    World   from   C   " -> "Hello World from C"
   Why Asked: Tests read/write two-pointer indexing without secondary memory.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <ctype.h>

void trimAndRemoveConsecutiveSpaces(char *str) {
    if (!str) return;

    int r = 0, w = 0;
    int in_space = 0;

    // 1. Skip leading whitespace
    while (str[r] == ' ' || str[r] == '\t') {
        r++;
    }

    // 2. Compact single space between words
    while (str[r] != '\0') {
        if (str[r] == ' ' || str[r] == '\t') {
            if (!in_space) {
                str[w++] = ' ';
                in_space = 1;
            }
        } else {
            str[w++] = str[r];
            in_space = 0;
        }
        r++;
    }

    // 3. Remove trailing space if any
    if (w > 0 && str[w - 1] == ' ') {
        w--;
    }

    str[w] = '\0';
}

int main() {
    char msg[] = "   Hello    World   from   C   ";
    printf("Before: \"%s\"\n", msg);
    trimAndRemoveConsecutiveSpaces(msg);
    printf("After:  \"%s\"\n", msg);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q10. One Edit Away (Insert, Remove, or Replace a Character)
   Difficulty: Medium | Time: O(N), Space: O(1)
   Example: ("pale", "ple") -> 1, ("pales", "pale") -> 1, ("pale", "bake") -> 0
   Why Asked: Tests string length comparison, two-pointer alignment, and early termination.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isOneEditAway(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;

    int len1 = strlen(s1);
    int len2 = strlen(s2);

    if (abs(len1 - len2) > 1) return 0;

    // Ensure s1 is shorter or equal to s2
    const char *short_str = (len1 < len2) ? s1 : s2;
    const char *long_str  = (len1 < len2) ? s2 : s1;

    int i = 0, j = 0;
    int diff_found = 0;

    while (*(short_str + i) && *(long_str + j)) {
        if (*(short_str + i) != *(long_str + j)) {
            if (diff_found) return 0; // More than 1 edit
            diff_found = 1;

            if (len1 == len2) {
                // Replacement: advance both
                i++;
                j++;
            } else {
                // Insertion in long_str: advance long_str only
                j++;
            }
        } else {
            i++;
            j++;
        }
    }

    return 1;
}

int main() {
    printf("pale, ple:   %d\n", isOneEditAway("pale", "ple"));   // 1
    printf("pales, pale: %d\n", isOneEditAway("pales", "pale")); // 1
    printf("pale, bale:  %d\n", isOneEditAway("pale", "bale"));  // 1
    printf("pale, bake:  %d\n", isOneEditAway("pale", "bake"));  // 0
    return 0;
}
*/


/* ============================================================================
   SECTION 2: MEMORY OPERATIONS & POINTER INTERNALS (Q11 - Q17)
   ============================================================================ */

/* ----------------------------------------------------------------------------
   Q11. Implement my_memmove (Safely Handling Overlapping Memory Regions)
   Difficulty: Medium-Hard | Time: O(N), Space: O(1)
   Why Asked: Classic interview question testing byte pointer casting and
              distinguishing forward copying (dst <= src) from backward copying (dst > src).
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stddef.h>

void* my_memmove(void *dest, const void *src, size_t n) {
    if (!dest || !src || n == 0 || dest == src) {
        return dest;
    }

    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    if (d < s) {
        // Non-overlapping or dest is before src: Copy forward (left to right)
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        // Dest is after src (overlap potential): Copy backward (right to left)
        for (size_t i = n; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}

int main() {
    char str[32] = "ABCDEFGHIJK";
    // Overlapping move: shift "CDEF" 2 bytes to the right into "EFGH"
    my_memmove(str + 4, str + 2, 4);
    printf("Result: %s\n", str); // Expect: "ABCDCDEFIJK"
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q12. Implement my_memcpy with Word-Aligned Fast Copying
   Difficulty: Medium | Time: O(N), Space: O(1)
   Why Asked: Product company interviews look for optimization knowledge:
              copying byte-by-byte until aligned to 4/8 byte boundaries, then
              copying in machine word chunks (uint32_t / uint64_t), then trailing bytes.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

void* my_memcpy(void *dest, const void *src, size_t n) {
    if (!dest || !src || n == 0 || dest == src) return dest;

    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    // 1. Copy leading unaligned bytes until dest pointer is aligned to 4-byte boundary
    while (n > 0 && ((uintptr_t)d & 0x3) != 0) {
        *d++ = *s++;
        n--;
    }

    // 2. Fast copy 32-bit (4-byte) words if both pointers are suitably aligned
    if (((uintptr_t)s & 0x3) == 0) {
        uint32_t *d32 = (uint32_t *)d;
        const uint32_t *s32 = (const uint32_t *)s;
        while (n >= 4) {
            *d32++ = *s32++;
            n -= 4;
        }
        d = (uint8_t *)d32;
        s = (const uint8_t *)s32;
    }

    // 3. Copy any remaining trailing bytes
    while (n > 0) {
        *d++ = *s++;
        n--;
    }

    return dest;
}

int main() {
    char src[] = "High-Performance Systems Programming in C";
    char dst[64];
    my_memcpy(dst, src, sizeof(src));
    printf("Copied: \"%s\"\n", dst);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q13. Implement my_memset with 32-bit Word Fill Optimization
   Difficulty: Easy-Medium | Time: O(N), Space: O(1)
   Why Asked: Tests byte replication into 32-bit word (`c | (c<<8) | ...`)
              and word-sized memory writes.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

void* my_memset(void *s, int c, size_t n) {
    if (!s || n == 0) return s;

    uint8_t *byte_ptr = (uint8_t *)s;
    uint8_t byte_val = (uint8_t)c;

    // Fill leading unaligned bytes
    while (n > 0 && ((uintptr_t)byte_ptr & 0x3) != 0) {
        *byte_ptr++ = byte_val;
        n--;
    }

    // Construct 32-bit word filled with byte_val
    uint32_t word_val = ((uint32_t)byte_val) |
                        (((uint32_t)byte_val) << 8) |
                        (((uint32_t)byte_val) << 16) |
                        (((uint32_t)byte_val) << 24);

    uint32_t *word_ptr = (uint32_t *)byte_ptr;
    while (n >= 4) {
        *word_ptr++ = word_val;
        n -= 4;
    }

    // Fill trailing bytes
    byte_ptr = (uint8_t *)word_ptr;
    while (n > 0) {
        *byte_ptr++ = byte_val;
        n--;
    }

    return s;
}

int main() {
    int arr[10];
    my_memset(arr, 0, sizeof(arr));
    for (int i = 0; i < 10; i++) printf("%d ", arr[i]);
    printf("\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q14. Dynamic 2D Array Allocation & Deallocation in Pure C
   Difficulty: Medium | Time: O(Rows * Cols), Space: O(Rows * Cols)
   Why Asked: Top companies evaluate whether a candidate knows the trade-off between:
              Method A: Single contiguous malloc (cache friendly, 1 free call)
              Method B: Array of row pointers (non-contiguous, requires loop to free)
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

// Method A: Contiguous single block allocation with row pointer array
int** allocate2DContiguous(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;

    // Allocate array of pointers
    int **matrix = (int **)malloc(rows * sizeof(int *));
    if (!matrix) return NULL;

    // Allocate data elements in a single contiguous block
    matrix[0] = (int *)malloc(rows * cols * sizeof(int));
    if (!matrix[0]) {
        free(matrix);
        return NULL;
    }

    // Set row pointers
    for (int i = 1; i < rows; i++) {
        matrix[i] = matrix[0] + i * cols;
    }

    return matrix;
}

void free2DContiguous(int **matrix) {
    if (!matrix) return;
    free(matrix[0]); // Free contiguous data block
    free(matrix);    // Free row pointer array
}

int main() {
    int rows = 3, cols = 4;
    int **mat = allocate2DContiguous(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[i][j] = (i + 1) * 10 + j;
            printf("%3d ", mat[i][j]);
        }
        printf("\n");
    }

    free2DContiguous(mat);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q15. Implement aligned_malloc and aligned_free
   Difficulty: Hard | Time: O(1), Space: O(1)
   Why Asked: Essential for performance-critical systems (DMA, SIMD, AVX, cache-lines).
              Tests pointer arithmetic, bitwise alignment, and saving the original
              pointer offset immediately preceding the aligned address.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void* aligned_malloc(size_t size, size_t alignment) {
    // Alignment must be a power of 2 and at least sizeof(void*)
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        return NULL;
    }

    // Extra space needed: (alignment - 1) for adjustment + sizeof(void*) to store raw pointer
    size_t total_size = size + (alignment - 1) + sizeof(void *);
    void *raw_ptr = malloc(total_size);
    if (!raw_ptr) return NULL;

    // Compute aligned address after reserving room for the pointer storage
    uintptr_t raw_addr = (uintptr_t)raw_ptr + sizeof(void *);
    uintptr_t aligned_addr = (raw_addr + (alignment - 1)) & ~(alignment - 1);

    // Store raw pointer just before aligned address
    void **header = (void **)aligned_addr - 1;
    *header = raw_ptr;

    return (void *)aligned_addr;
}

void aligned_free(void *ptr) {
    if (!ptr) return;
    // Retrieve the original raw pointer stored right before aligned address
    void **header = (void **)ptr - 1;
    void *raw_ptr = *header;
    free(raw_ptr);
}

int main() {
    size_t alignment = 64; // e.g. 64-byte cache line alignment
    int *p = (int *)aligned_malloc(100 * sizeof(int), alignment);
    printf("Allocated aligned address: %p\n", (void*)p);
    printf("Check alignment (addr %% %zu): %zu\n", alignment, (uintptr_t)p % alignment);
    aligned_free(p);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q16. Generic Swap Function in C for Any Data Type
   Difficulty: Easy-Medium | Time: O(size), Space: O(size)
   Why Asked: Demonstrates deep knowledge of void*, type independence, and
              byte-level buffer swapping.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generic_swap(void *a, void *b, size_t size) {
    if (!a || !b || a == b || size == 0) return;

    // Small stack buffer for common types to avoid dynamic allocation
    unsigned char buffer[128];
    unsigned char *temp = buffer;

    if (size > sizeof(buffer)) {
        temp = (unsigned char *)malloc(size);
        if (!temp) return; // Allocation failed
    }

    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);

    if (temp != buffer) {
        free(temp);
    }
}

typedef struct {
    int id;
    char name[20];
} Employee;

int main() {
    double x = 3.14, y = 9.81;
    generic_swap(&x, &y, sizeof(double));
    printf("x: %.2f, y: %.2f\n", x, y);

    Employee e1 = {101, "Alice"};
    Employee e2 = {102, "Bob"};
    generic_swap(&e1, &e2, sizeof(Employee));
    printf("e1: %s, e2: %s\n", e1.name, e2.name);

    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q17. Linux Kernel offsetof and container_of Macros
   Difficulty: Medium-Hard | Time: O(1), Space: O(1)
   Why Asked: Fundamental C pattern used extensively in production system code
              (Linux kernel, drivers, intrusive data structures).
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stddef.h>

#define my_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define my_container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - my_offsetof(type, member) ); \
})

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct Packet {
    int packet_id;
    int payload_len;
    Node list_node; // Intrusive member
    char payload[64];
} Packet;

int main() {
    Packet pkt;
    pkt.packet_id = 42;
    pkt.payload_len = 100;

    // Given only a pointer to the intrusive member list_node:
    Node *node_ptr = &pkt.list_node;

    // Recover the enclosing Packet pointer
    Packet *recovered_pkt = my_container_of(node_ptr, Packet, list_node);

    printf("Offset of list_node: %zu\n", my_offsetof(Packet, list_node));
    printf("Recovered Packet ID: %d\n", recovered_pkt->packet_id);
    printf("Pointers match: %s\n", (&pkt == recovered_pkt) ? "YES" : "NO");

    return 0;
}
*/


/* ============================================================================
   SECTION 3: 2D MATRICES & MULTI-DIMENSIONAL ARRAYS (Q18 - Q22)
   ============================================================================ */

/* ----------------------------------------------------------------------------
   Q18. Rotate an N x N Matrix by 90 Degrees Clockwise In-Place
   Difficulty: Medium | Time: O(N^2), Space: O(1)
   Why Asked: Tests 2D index coordinate manipulation and in-place transformations.
              Strategy: 1. Transpose the matrix; 2. Reverse each row.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

void rotateMatrix90Clockwise(int n, int mat[n][n]) {
    // Step 1: Transpose matrix (swap mat[i][j] with mat[j][i])
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int temp = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = temp;
        }
    }

    // Step 2: Reverse each row
    for (int i = 0; i < n; i++) {
        int left = 0, right = n - 1;
        while (left < right) {
            int temp = mat[i][left];
            mat[i][left] = mat[i][right];
            mat[i][right] = temp;
            left++;
            right--;
        }
    }
}

void printMatrix(int n, int mat[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", mat[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int mat[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    printf("Original Matrix:\n");
    printMatrix(3, mat);
    rotateMatrix90Clockwise(3, mat);
    printf("Rotated 90 Deg Clockwise:\n");
    printMatrix(3, mat);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q19. Spiral Order Traversal of an M x N Matrix
   Difficulty: Medium | Time: O(M * N), Space: O(1)
   Why Asked: Tests boundary management (top, bottom, left, right) and preventing
              duplicate visits when bounds converge.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

void printSpiralOrder(int rows, int cols, int mat[rows][cols]) {
    int top = 0, bottom = rows - 1;
    int left = 0, right = cols - 1;

    printf("Spiral Order: ");
    while (top <= bottom && left <= right) {
        // 1. Traverse from Left to Right along Top row
        for (int j = left; j <= right; j++) {
            printf("%d ", mat[top][j]);
        }
        top++;

        // 2. Traverse from Top to Bottom along Right column
        for (int i = top; i <= bottom; i++) {
            printf("%d ", mat[i][right]);
        }
        right--;

        // 3. Traverse from Right to Left along Bottom row (if valid)
        if (top <= bottom) {
            for (int j = right; j >= left; j--) {
                printf("%d ", mat[bottom][j]);
            }
            bottom--;
        }

        // 4. Traverse from Bottom to Top along Left column (if valid)
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                printf("%d ", mat[i][left]);
            }
            left++;
        }
    }
    printf("\n");
}

int main() {
    int mat[3][4] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12}
    };
    printSpiralOrder(3, 4, mat);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q20. Search in a Row-wise and Column-wise Sorted 2D Matrix
   Difficulty: Medium | Time: O(M + N), Space: O(1)
   Why Asked: Demonstrates pruning search space by starting from top-right or
              bottom-left corner rather than brute-forcing O(M*N).
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

int searchMatrixRowColSorted(int rows, int cols, int mat[rows][cols], int target, int *out_r, int *out_c) {
    // Start at top-right corner: (row 0, col cols - 1)
    int r = 0;
    int c = cols - 1;

    while (r < rows && c >= 0) {
        if (mat[r][c] == target) {
            if (out_r) *out_r = r;
            if (out_c) *out_c = c;
            return 1; // Found
        } else if (mat[r][c] > target) {
            c--; // Target is smaller, move left
        } else {
            r++; // Target is larger, move down
        }
    }

    return 0; // Not found
}

int main() {
    int mat[4][4] = {
        {10, 20, 30, 40},
        {15, 25, 35, 45},
        {27, 29, 37, 48},
        {32, 33, 39, 50}
    };
    int r = -1, c = -1;
    int target = 29;
    if (searchMatrixRowColSorted(4, 4, mat, target, &r, &c)) {
        printf("Element %d found at row %d, col %d\n", target, r, c);
    } else {
        printf("Element %d not found\n", target);
    }
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q21. Set Matrix Zeroes in O(1) Extra Space
   Difficulty: Medium-Hard | Time: O(M * N), Space: O(1)
   Why Asked: Tests using existing matrix storage (first row and first column) as
              flag indicators while tracking row0 and col0 separately.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

void setMatrixZeroes(int rows, int cols, int mat[rows][cols]) {
    int first_row_zero = 0;
    int first_col_zero = 0;

    // 1. Check if first column needs to be zeroed
    for (int i = 0; i < rows; i++) {
        if (mat[i][0] == 0) {
            first_col_zero = 1;
            break;
        }
    }

    // 2. Check if first row needs to be zeroed
    for (int j = 0; j < cols; j++) {
        if (mat[0][j] == 0) {
            first_row_zero = 1;
            break;
        }
    }

    // 3. Use first row & col as markers for the rest of the matrix
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            if (mat[i][j] == 0) {
                mat[i][0] = 0;
                mat[0][j] = 0;
            }
        }
    }

    // 4. Update inner cells based on markers
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            if (mat[i][0] == 0 || mat[0][j] == 0) {
                mat[i][j] = 0;
            }
        }
    }

    // 5. Zero out first row if needed
    if (first_row_zero) {
        for (int j = 0; j < cols; j++) mat[0][j] = 0;
    }

    // 6. Zero out first col if needed
    if (first_col_zero) {
        for (int i = 0; i < rows; i++) mat[i][0] = 0;
    }
}

int main() {
    int mat[3][3] = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    };
    setMatrixZeroes(3, 3, mat);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) printf("%d ", mat[i][j]);
        printf("\n");
    }
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q22. Matrix Multiplication with Dynamic Memory & Dimension Check
   Difficulty: Easy-Medium | Time: O(R1 * C1 * C2), Space: O(R1 * C2)
   Why Asked: Tests standard triple-loop matrix multiplication, dynamic 2D
              pointer allocation, validation (cols1 == rows2), and proper cleanup.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

int** multiplyMatrices(int **A, int r1, int c1, int **B, int r2, int c2) {
    if (!A || !B || c1 != r2 || r1 <= 0 || c1 <= 0 || c2 <= 0) {
        return NULL;
    }

    // Allocate result matrix (r1 x c2)
    int **res = (int **)malloc(r1 * sizeof(int *));
    if (!res) return NULL;
    for (int i = 0; i < r1; i++) {
        res[i] = (int *)calloc(c2, sizeof(int));
        if (!res[i]) {
            // Free partially allocated memory on error
            for (int k = 0; k < i; k++) free(res[k]);
            free(res);
            return NULL;
        }
    }

    // Compute C = A * B
    for (int i = 0; i < r1; i++) {
        for (int k = 0; k < c1; k++) {
            int a_ik = A[i][k];
            for (int j = 0; j < c2; j++) {
                res[i][j] += a_ik * B[k][j];
            }
        }
    }

    return res;
}

int main() {
    // Demonstration
    int r1 = 2, c1 = 3, r2 = 3, c2 = 2;
    int a_data[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int b_data[3][2] = {{7, 8}, {9, 1}, {2, 3}};

    int *A[2] = {a_data[0], a_data[1]};
    int *B[3] = {b_data[0], b_data[1], b_data[2]};

    int **C = multiplyMatrices(A, r1, c1, B, r2, c2);
    if (C) {
        printf("Result Matrix (%dx%d):\n", r1, c2);
        for (int i = 0; i < r1; i++) {
            for (int j = 0; j < c2; j++) printf("%4d ", C[i][j]);
            printf("\n");
            free(C[i]);
        }
        free(C);
    }
    return 0;
}
*/


/* ============================================================================
   SECTION 4: ADVANCED BIT MANIPULATION (3 YoE INTERVIEW LEVEL) (Q23 - Q32)
   ============================================================================ */

/* ----------------------------------------------------------------------------
   Q23. Fast Parallel Reverse Bits of a 32-bit Integer in O(log N)
   Difficulty: Medium | Time: O(1) - 5 operations, Space: O(1)
   Why Asked: Reversing bit-by-bit in a loop takes 32 cycles. Top product companies
              expect divide-and-conquer bitmask swapping (16, 8, 4, 2, 1).
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>

uint32_t reverseBitsFast(uint32_t n) {
    // Swap 16-bit halves
    n = (n >> 16) | (n << 16);
    // Swap 8-bit bytes
    n = ((n & 0xFF00FF00) >> 8) | ((n & 0x00FF00FF) << 8);
    // Swap 4-bit nibbles
    n = ((n & 0xF0F0F0F0) >> 4) | ((n & 0x0F0F0F0F) << 4);
    // Swap 2-bit pairs
    n = ((n & 0xCCCCCCCC) >> 2) | ((n & 0x33333333) << 2);
    // Swap adjacent 1-bit bits
    n = ((n & 0xAAAAAAAA) >> 1) | ((n & 0x55555555) << 1);
    return n;
}

int main() {
    uint32_t val = 0x00000001; // 1 -> MSB set: 0x80000000
    printf("Original: 0x%08X\n", val);
    printf("Reversed: 0x%08X\n", reverseBitsFast(val));
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q24. Swap Odd and Even Bits of an Integer in a Single Line
   Difficulty: Easy-Medium | Time: O(1), Space: O(1)
   Why Asked: Tests understanding of even/odd bitmasks (`0xAAAAAAAA` and `0x55555555`).
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>

uint32_t swapOddEvenBits(uint32_t x) {
    // 0xAAAAAAAA has all odd bits set (bits 1, 3, 5, ... 31)
    // 0x55555555 has all even bits set (bits 0, 2, 4, ... 30)
    return ((x & 0xAAAAAAAA) >> 1) | ((x & 0x55555555) << 1);
}

int main() {
    uint32_t x = 23; // Binary: 00010111 -> After swap: 00101011 = 43
    printf("x: %u, swapped: %u\n", x, swapOddEvenBits(x));
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q25. Swap High and Low Nibbles in an 8-bit Byte
   Difficulty: Easy | Time: O(1), Space: O(1)
   Why Asked: Very common in hardware driver/register config tests.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>

uint8_t swapNibbles(uint8_t b) {
    return (uint8_t)((b >> 4) | (b << 4));
}

int main() {
    uint8_t val = 0xAB;
    printf("Original: 0x%02X, Swapped: 0x%02X\n", val, swapNibbles(val)); // 0xBA
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q26. Find the Element That Appears Once (All Others Appear 3 Times)
   Difficulty: Medium-Hard | Time: O(N), Space: O(1)
   Why Asked: Pure XOR only cancels pairs. When elements appear 3 times, you must
              implement a two-state modulo-3 bit counter using bitwise logic.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

int findSingleNumberOutOfTriples(const int arr[], int size) {
    int ones = 0; // Tracks bits seen 1 time (mod 3)
    int twos = 0; // Tracks bits seen 2 times (mod 3)

    for (int i = 0; i < size; i++) {
        // 'twos' takes bits that were in 'ones' and also appeared in current element
        twos |= (ones & arr[i]);
        // 'ones' toggles bits from current element
        ones ^= arr[i];

        // When a bit appears 3 times, it is set in both 'ones' and 'twos'
        int common_three = ones & twos;

        // Clear bits that have appeared 3 times
        ones &= ~common_three;
        twos &= ~common_three;
    }

    return ones;
}

int main() {
    int arr[] = {6, 1, 3, 3, 3, 6, 6}; // 1 appears once
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Single element: %d\n", findSingleNumberOutOfTriples(arr, n)); // 1
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q27. Check if an Integer Has an Alternating Bit Pattern
   Difficulty: Easy-Medium | Time: O(1), Space: O(1)
   Example: 5 (101 in binary) -> True, 7 (111 in binary) -> False, 10 (1010) -> True
   Why Asked: Tests mathematical deduction: if bits alternate, (n ^ (n >> 1)) produces
              all 1s. Then (all_ones & (all_ones + 1)) must equal 0.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

int hasAlternatingBits(unsigned int n) {
    // If bits alternate (e.g., 10101), then n ^ (n >> 1) yields 11111
    unsigned int all_ones = n ^ (n >> 1);
    // Check if all_ones is of the form 2^k - 1: (x & (x + 1)) == 0
    return (all_ones & (all_ones + 1)) == 0;
}

int main() {
    printf("5 (101b):  %d\n", hasAlternatingBits(5));  // 1
    printf("7 (111b):  %d\n", hasAlternatingBits(7));  // 0
    printf("10 (1010b): %d\n", hasAlternatingBits(10)); // 1
    printf("11 (1011b): %d\n", hasAlternatingBits(11)); // 0
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q28. Count Trailing Zeros (CTZ) Without Built-in Functions
   Difficulty: Medium | Time: O(1) - Binary Search, Space: O(1)
   Why Asked: Critical for priority interrupt decoding and bitfield search.
              Binary search checks 16, 8, 4, 2, 1 bits.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>

int countTrailingZeros(uint32_t x) {
    if (x == 0) return 32;

    int count = 0;

    // Check if lowest 16 bits are zero
    if ((x & 0x0000FFFF) == 0) { count += 16; x >>= 16; }
    // Check if lowest 8 bits are zero
    if ((x & 0x000000FF) == 0) { count += 8;  x >>= 8;  }
    // Check if lowest 4 bits are zero
    if ((x & 0x0000000F) == 0) { count += 4;  x >>= 4;  }
    // Check if lowest 2 bits are zero
    if ((x & 0x00000003) == 0) { count += 2;  x >>= 2;  }
    // Check if lowest 1 bit is zero
    if ((x & 0x00000001) == 0) { count += 1; }

    return count;
}

int main() {
    printf("CTZ(16 = 0x10): %d\n", countTrailingZeros(16)); // 4
    printf("CTZ(1 = 0x01):  %d\n", countTrailingZeros(1));  // 0
    printf("CTZ(80 = 0x50): %d\n", countTrailingZeros(80)); // 4
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q29. Count Leading Zeros (CLZ) Without Built-in Functions
   Difficulty: Medium | Time: O(1) - Binary Search, Space: O(1)
   Why Asked: Used for finding MSB position, floating point normalization,
              and priority scheduling.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>

int countLeadingZeros(uint32_t x) {
    if (x == 0) return 32;

    int count = 0;

    // Check if highest 16 bits are zero
    if ((x & 0xFFFF0000) == 0) { count += 16; x <<= 16; }
    // Check if highest 8 bits are zero
    if ((x & 0xFF000000) == 0) { count += 8;  x <<= 8;  }
    // Check if highest 4 bits are zero
    if ((x & 0xF0000000) == 0) { count += 4;  x <<= 4;  }
    // Check if highest 2 bits are zero
    if ((x & 0xC0000000) == 0) { count += 2;  x <<= 2;  }
    // Check if highest 1 bit is zero
    if ((x & 0x80000000) == 0) { count += 1; }

    return count;
}

int main() {
    printf("CLZ(1):          %d\n", countLeadingZeros(1));          // 31
    printf("CLZ(0x80000000): %d\n", countLeadingZeros(0x80000000)); // 0
    printf("CLZ(16):         %d\n", countLeadingZeros(16));         // 27
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q30. Round Up to the Next Power of Two Using Bit Smearing
   Difficulty: Easy-Medium | Time: O(1), Space: O(1)
   Why Asked: Fundamental for sizing circular buffers, hash tables, and memory blocks.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>

uint32_t nextPowerOfTwo(uint32_t v) {
    if (v == 0) return 1;

    // Decrement so that if v is already a power of 2, it returns v
    v--;
    // Propagate highest set bit to all lower positions
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++; // Add 1 to roll over into next power of 2

    return v;
}

int main() {
    printf("Next pow2(5):  %u\n", nextPowerOfTwo(5));  // 8
    printf("Next pow2(16): %u\n", nextPowerOfTwo(16)); // 16
    printf("Next pow2(17): %u\n", nextPowerOfTwo(17)); // 32
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q31. Compute Absolute Value of an Integer Without Branching
   Difficulty: Easy | Time: O(1), Space: O(1)
   Why Asked: Tests knowledge of two's complement sign extension arithmetic right shift.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

int branchlessAbs(int n) {
    // Arithmetic right shift by 31 bits:
    // If n >= 0, mask = 0x00000000 (0)
    // If n < 0,  mask = 0xFFFFFFFF (-1)
    int mask = n >> 31;

    // Formula: (n + mask) ^ mask
    // For n >= 0: (n + 0) ^ 0 = n
    // For n < 0:  (n - 1) ^ -1 = -n (Two's complement negation!)
    return (n + mask) ^ mask;
}

int main() {
    printf("abs(-42): %d\n", branchlessAbs(-42)); // 42
    printf("abs(100): %d\n", branchlessAbs(100)); // 100
    printf("abs(0):   %d\n", branchlessAbs(0));   // 0
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q32. Branchless Minimum and Maximum of Two Integers
   Difficulty: Easy | Time: O(1), Space: O(1)
   Why Asked: Branch prediction misses carry a 15-20 cycle penalty on modern CPUs.
              Branchless math is favored in high-throughput data processing.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

int branchlessMin(int x, int y) {
    // If x < y: (x - y) sign bit is 1, -(x < y) is -1 (all 1s)
    // return y ^ ((x ^ y) & -1) = y ^ x ^ y = x
    // If x >= y: -(x < y) is 0
    // return y ^ 0 = y
    return y ^ ((x ^ y) & -(x < y));
}

int branchlessMax(int x, int y) {
    return x ^ ((x ^ y) & -(x < y));
}

int main() {
    int a = 15, b = 42;
    printf("Min(%d, %d): %d\n", a, b, branchlessMin(a, b)); // 15
    printf("Max(%d, %d): %d\n", a, b, branchlessMax(a, b)); // 42
    return 0;
}
*/


/* ============================================================================
   SECTION 5: ARRAY ALGORITHMS & TWO-POINTERS (Q33 - Q39)
   ============================================================================ */

/* ----------------------------------------------------------------------------
   Q33. Two Sum on a Sorted Array (Two-Pointer Technique)
   Difficulty: Easy | Time: O(N), Space: O(1)
   Why Asked: Tests two-pointer convergence from left and right.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

int findTwoSumSorted(const int arr[], int size, int target, int *idx1, int *idx2) {
    if (!arr || size < 2) return 0;

    int left = 0;
    int right = size - 1;

    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == target) {
            if (idx1) *idx1 = left;
            if (idx2) *idx2 = right;
            return 1;
        } else if (sum < target) {
            left++;
        } else {
            right--;
        }
    }

    return 0;
}

int main() {
    int sorted_arr[] = {2, 7, 11, 15, 18, 22};
    int n = sizeof(sorted_arr) / sizeof(sorted_arr[0]);
    int i1, i2;
    if (findTwoSumSorted(sorted_arr, n, 25, &i1, &i2)) {
        printf("Pair found at indices: %d and %d (values: %d + %d = 25)\n",
               i1, i2, sorted_arr[i1], sorted_arr[i2]);
    }
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q34. Three Sum (Find All Unique Triplets Summing to Zero)
   Difficulty: Medium | Time: O(N^2), Space: O(1) auxiliary
   Why Asked: Tests sorting followed by two-pointer search with duplicate pruning.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

static int compareInts(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void threeSum(int arr[], int n) {
    if (!arr || n < 3) return;

    // Sort array first
    qsort(arr, n, sizeof(int), compareInts);

    printf("Triplets with sum 0:\n");
    for (int i = 0; i < n - 2; i++) {
        // Skip duplicate values for first element
        if (i > 0 && arr[i] == arr[i - 1]) continue;

        int left = i + 1;
        int right = n - 1;
        int target = -arr[i];

        while (left < right) {
            int sum = arr[left] + arr[right];
            if (sum == target) {
                printf("[%d, %d, %d]\n", arr[i], arr[left], arr[right]);

                // Skip duplicates for second and third elements
                while (left < right && arr[left] == arr[left + 1]) left++;
                while (left < right && arr[right] == arr[right - 1]) right--;

                left++;
                right--;
            } else if (sum < target) {
                left++;
            } else {
                right--;
            }
        }
    }
}

int main() {
    int arr[] = {-1, 0, 1, 2, -1, -4};
    int n = sizeof(arr) / sizeof(arr[0]);
    threeSum(arr, n);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q35. Product of Array Except Self Without Division Operator
   Difficulty: Medium | Time: O(N), Space: O(1) extra (excluding output array)
   Why Asked: Tests prefix and suffix product passes. The constraint "no division"
              prevents calculating total product and dividing by arr[i] (which
              also avoids division-by-zero traps).
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

void productExceptSelf(const int arr[], int n, int output[]) {
    if (!arr || !output || n <= 0) return;

    // Step 1: Prefix products into output array
    output[0] = 1;
    for (int i = 1; i < n; i++) {
        output[i] = output[i - 1] * arr[i - 1];
    }

    // Step 2: Multiply with running suffix product from right to left
    int suffix = 1;
    for (int i = n - 1; i >= 0; i--) {
        output[i] = output[i] * suffix;
        suffix *= arr[i];
    }
}

int main() {
    int arr[] = {1, 2, 3, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    int out[4];
    productExceptSelf(arr, n, out);
    printf("Product array: ");
    for (int i = 0; i < n; i++) printf("%d ", out[i]); // 24 12 8 6
    printf("\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q36. Merge Overlapping Intervals
   Difficulty: Medium | Time: O(N log N), Space: O(1) in-place
   Why Asked: Classic problem testing struct definition, qsort comparison callback,
              and in-place interval coalescing.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} Interval;

static int compareIntervals(const void *a, const void *b) {
    Interval *i1 = (Interval *)a;
    Interval *i2 = (Interval *)b;
    return i1->start - i2->start;
}

int mergeIntervals(Interval intervals[], int n) {
    if (!intervals || n <= 1) return n;

    // Sort intervals by start time
    qsort(intervals, n, sizeof(Interval), compareIntervals);

    int write_idx = 0;

    for (int i = 1; i < n; i++) {
        // If current interval overlaps with previous merged interval
        if (intervals[i].start <= intervals[write_idx].end) {
            // Expand the merged interval's end if needed
            if (intervals[i].end > intervals[write_idx].end) {
                intervals[write_idx].end = intervals[i].end;
            }
        } else {
            // No overlap: Move write index forward
            write_idx++;
            intervals[write_idx] = intervals[i];
        }
    }

    return write_idx + 1; // Return count of merged intervals
}

int main() {
    Interval list[] = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
    int n = sizeof(list) / sizeof(list[0]);
    int new_size = mergeIntervals(list, n);

    printf("Merged Intervals:\n");
    for (int i = 0; i < new_size; i++) {
        printf("[%d, %d] ", list[i].start, list[i].end);
    }
    printf("\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q37. Minimum Number of Platforms Required for Railway Station
   Difficulty: Medium | Time: O(N log N), Space: O(1)
   Why Asked: Frequently asked at Cisco/Broadcom/Qualcomm. Given arrival and departure
              times, determine max concurrent trains at the station.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

static int compInt(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int findMinPlatforms(int arr[], int dep[], int n) {
    if (!arr || !dep || n <= 0) return 0;

    // Sort arrival and departure arrays independently
    qsort(arr, n, sizeof(int), compInt);
    qsort(dep, n, sizeof(int), compInt);

    int platforms_needed = 1;
    int max_platforms = 1;
    int i = 1; // Arrival pointer
    int j = 0; // Departure pointer

    while (i < n && j < n) {
        // If train arrives before previous departs, we need another platform
        if (arr[i] <= dep[j]) {
            platforms_needed++;
            if (platforms_needed > max_platforms) {
                max_platforms = platforms_needed;
            }
            i++;
        } else {
            // A train departed, free a platform
            platforms_needed--;
            j++;
        }
    }

    return max_platforms;
}

int main() {
    int arr[] = {900, 940, 950, 1100, 1500, 1800};
    int dep[] = {910, 1200, 1120, 1130, 1900, 2000};
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Min Platforms Required: %d\n", findMinPlatforms(arr, dep, n)); // 3
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q38. Rearrange Array in Alternating Max-Min Form in O(1) Extra Space
   Difficulty: Medium-Hard | Time: O(N), Space: O(1)
   Example: [1, 2, 3, 4, 5, 6] -> [6, 1, 5, 2, 4, 3]
   Why Asked: Tests mathematical trick: storing two values at a single array slot
              using `arr[i] += (new_value % M) * M`, where M > max_element.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

void rearrangeMaxMin(int arr[], int n) {
    if (!arr || n <= 1) return;

    int max_idx = n - 1;
    int min_idx = 0;
    int M = arr[n - 1] + 1; // Multiplier greater than any element

    for (int i = 0; i < n; i++) {
        // At even index, we want the current max element
        if (i % 2 == 0) {
            arr[i] += (arr[max_idx] % M) * M;
            max_idx--;
        } else {
            // At odd index, we want the current min element
            arr[i] += (arr[min_idx] % M) * M;
            min_idx++;
        }
    }

    // Decode: Divide each element by M to retrieve new values
    for (int i = 0; i < n; i++) {
        arr[i] = arr[i] / M;
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);
    rearrangeMaxMin(arr, n);
    printf("Rearranged: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]); // 7 1 6 2 5 3 4
    printf("\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q39. Find the Missing and Repeating Number in Array of 1 to N
   Difficulty: Medium | Time: O(N), Space: O(1)
   Why Asked: Tests array element negation as an in-place visited hash marker,
              avoiding extra memory allocation.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

void findMissingAndRepeating(int arr[], int n, int *repeating, int *missing) {
    *repeating = -1;
    *missing = -1;

    // Step 1: Use value as index; negate visited elements
    for (int i = 0; i < n; i++) {
        int val = abs(arr[i]);
        int idx = val - 1; // Convert 1-based to 0-based

        if (arr[idx] < 0) {
            *repeating = val; // Already negated: this is the duplicate
        } else {
            arr[idx] = -arr[idx];
        }
    }

    // Step 2: Element with positive value was never visited: that index + 1 is missing
    for (int i = 0; i < n; i++) {
        if (arr[i] > 0) {
            *missing = i + 1;
        } else {
            arr[i] = -arr[i]; // Restore original array
        }
    }
}

int main() {
    int arr[] = {3, 1, 3}; // 1 to 3: 3 is repeating, 2 is missing
    int rep, mis;
    findMissingAndRepeating(arr, 3, &rep, &mis);
    printf("Repeating: %d, Missing: %d\n", rep, mis);
    return 0;
}
*/


/* ============================================================================
   SECTION 6: ADVANCED LINKED LIST OPERATIONS (Q40 - Q44)
   ============================================================================ */

/* ----------------------------------------------------------------------------
   Q40. Add Two Numbers Represented by Linked Lists
   Difficulty: Medium | Time: O(max(N, M)), Space: O(max(N, M))
   Example: (2 -> 4 -> 3) + (5 -> 6 -> 4) = (7 -> 0 -> 8) [342 + 465 = 807]
   Why Asked: Tests node creation, carry calculation across nodes, and handling
              lists of differing lengths.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

ListNode* createListNode(int val) {
    ListNode *n = (ListNode *)malloc(sizeof(ListNode));
    n->val = val;
    n->next = NULL;
    return n;
}

ListNode* addTwoNumbers(ListNode *l1, ListNode *l2) {
    ListNode dummy;
    dummy.next = NULL;
    ListNode *tail = &dummy;
    int carry = 0;

    while (l1 != NULL || l2 != NULL || carry != 0) {
        int sum = carry;
        if (l1 != NULL) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2 != NULL) {
            sum += l2->val;
            l2 = l2->next;
        }

        carry = sum / 10;
        tail->next = createListNode(sum % 10);
        tail = tail->next;
    }

    return dummy.next;
}

void printLinkedList(ListNode *head) {
    while (head) {
        printf("%d%s", head->val, head->next ? " -> " : "");
        head = head->next;
    }
    printf("\n");
}

int main() {
    // 342: 2 -> 4 -> 3
    ListNode *l1 = createListNode(2);
    l1->next = createListNode(4);
    l1->next->next = createListNode(3);

    // 465: 5 -> 6 -> 4
    ListNode *l2 = createListNode(5);
    l2->next = createListNode(6);
    l2->next->next = createListNode(4);

    ListNode *res = addTwoNumbers(l1, l2);
    printf("Sum list (807): ");
    printLinkedList(res); // 7 -> 0 -> 8
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q41. Segregate Even and Odd Nodes in a Linked List
   Difficulty: Medium | Time: O(N), Space: O(1)
   Why Asked: Reorganize nodes so all even-value nodes appear before odd-value
              nodes, while strictly preserving their original relative order.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

ListNode* segregateEvenOdd(ListNode *head) {
    if (!head || !head->next) return head;

    ListNode evenDummy = {0, NULL};
    ListNode oddDummy  = {0, NULL};
    ListNode *evenTail = &evenDummy;
    ListNode *oddTail  = &oddDummy;

    ListNode *curr = head;
    while (curr != NULL) {
        if (curr->val % 2 == 0) {
            evenTail->next = curr;
            evenTail = evenTail->next;
        } else {
            oddTail->next = curr;
            oddTail = oddTail->next;
        }
        curr = curr->next;
    }

    // Connect even list to odd list
    evenTail->next = oddDummy.next;
    oddTail->next = NULL; // Terminate list

    return evenDummy.next ? evenDummy.next : oddDummy.next;
}

int main() {
    // 1 -> 2 -> 3 -> 4 -> 5 -> 6
    ListNode nodes[6];
    for (int i = 0; i < 6; i++) {
        nodes[i].val = i + 1;
        nodes[i].next = (i < 5) ? &nodes[i + 1] : NULL;
    }

    ListNode *res = segregateEvenOdd(&nodes[0]);
    while (res) {
        printf("%d ", res->val); // 2 4 6 1 3 5
        res = res->next;
    }
    printf("\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q42. Rearrange Linked List (Zip-Merge / Reorder List)
   Difficulty: Medium-Hard | Time: O(N), Space: O(1)
   Example: L0 -> L1 -> L2 -> L3 -> L4 -> L5  becomes  L0 -> L5 -> L1 -> L4 -> L2 -> L3
   Why Asked: Combines 3 core list algorithms in one:
              1. Find middle with slow/fast pointers
              2. Reverse second half
              3. Alternating merge of two lists
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

static ListNode* reverseSublist(ListNode *head) {
    ListNode *prev = NULL, *curr = head, *nxt = NULL;
    while (curr) {
        nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}

void reorderList(ListNode *head) {
    if (!head || !head->next || !head->next->next) return;

    // 1. Find middle node
    ListNode *slow = head, *fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 2. Split and reverse second half
    ListNode *second_half = reverseSublist(slow->next);
    slow->next = NULL;

    // 3. Interleave two halves
    ListNode *first_half = head;
    while (second_half != NULL) {
        ListNode *tmp1 = first_half->next;
        ListNode *tmp2 = second_half->next;

        first_half->next = second_half;
        second_half->next = tmp1;

        first_half = tmp1;
        second_half = tmp2;
    }
}

int main() {
    ListNode n[5];
    for (int i = 0; i < 5; i++) {
        n[i].val = i + 1;
        n[i].next = (i < 4) ? &n[i + 1] : NULL;
    }

    reorderList(&n[0]);
    ListNode *curr = &n[0];
    while (curr) {
        printf("%d ", curr->val); // 1 5 2 4 3
        curr = curr->next;
    }
    printf("\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q43. Flatten a Multilevel Doubly Linked List with Child Pointers
   Difficulty: Medium-Hard | Time: O(N), Space: O(1)
   Why Asked: Tests recursive or iterative tail-stitching logic without auxiliary memory.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

typedef struct MLNode {
    int val;
    struct MLNode *prev;
    struct MLNode *next;
    struct MLNode *child;
} MLNode;

MLNode* flattenMultilevelList(MLNode *head) {
    if (!head) return NULL;

    MLNode *curr = head;
    while (curr != NULL) {
        if (curr->child != NULL) {
            MLNode *next_node = curr->next;
            MLNode *child_head = curr->child;

            // Connect curr to child_head
            curr->next = child_head;
            child_head->prev = curr;
            curr->child = NULL;

            // Find tail of child list
            MLNode *child_tail = child_head;
            while (child_tail->next != NULL) {
                child_tail = child_tail->next;
            }

            // Connect child_tail to saved next_node
            child_tail->next = next_node;
            if (next_node) {
                next_node->prev = child_tail;
            }
        }
        curr = curr->next;
    }

    return head;
}

int main() {
    MLNode n1 = {1, NULL, NULL, NULL};
    MLNode n2 = {2, NULL, NULL, NULL};
    MLNode n3 = {3, NULL, NULL, NULL};
    MLNode c1 = {10, NULL, NULL, NULL};
    MLNode c2 = {20, NULL, NULL, NULL};

    n1.next = &n2; n2.prev = &n1;
    n2.next = &n3; n3.prev = &n2;
    n2.child = &c1;
    c1.next = &c2; c2.prev = &c1;

    MLNode *flat = flattenMultilevelList(&n1);
    while (flat) {
        printf("%d ", flat->val); // 1 2 10 20 3
        flat = flat->next;
    }
    printf("\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q44. Clone a Linked List with Next and Random Pointer in O(1) Extra Space
   Difficulty: Hard | Time: O(N), Space: O(1) auxiliary
   Why Asked: Avoids hash maps by interleaving cloned nodes directly after original nodes:
              Step 1: Create copy nodes: A -> A' -> B -> B' -> C -> C'
              Step 2: Assign random pointers: curr->next->random = curr->random->next
              Step 3: Separate original and cloned lists
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>

typedef struct NodeWithRandom {
    int val;
    struct NodeWithRandom *next;
    struct NodeWithRandom *random;
} NodeWithRandom;

NodeWithRandom* cloneListWithRandom(NodeWithRandom *head) {
    if (!head) return NULL;

    // Step 1: Create duplicate nodes and interleave them
    NodeWithRandom *curr = head;
    while (curr != NULL) {
        NodeWithRandom *copy = (NodeWithRandom *)malloc(sizeof(NodeWithRandom));
        copy->val = curr->val;
        copy->next = curr->next;
        copy->random = NULL;

        curr->next = copy;
        curr = copy->next;
    }

    // Step 2: Assign random pointers to copied nodes
    curr = head;
    while (curr != NULL) {
        if (curr->random != NULL) {
            curr->next->random = curr->random->next;
        }
        curr = curr->next->next;
    }

    // Step 3: Decouple the two interleaved lists
    curr = head;
    NodeWithRandom *cloned_head = head->next;
    NodeWithRandom *copy_curr = cloned_head;

    while (curr != NULL) {
        curr->next = curr->next->next;
        if (copy_curr->next != NULL) {
            copy_curr->next = copy_curr->next->next;
        }
        curr = curr->next;
        copy_curr = copy_curr->next;
    }

    return cloned_head;
}

int main() {
    NodeWithRandom n1 = {1, NULL, NULL};
    NodeWithRandom n2 = {2, NULL, NULL};
    NodeWithRandom n3 = {3, NULL, NULL};

    n1.next = &n2; n2.next = &n3;
    n1.random = &n3; // 1's random -> 3
    n2.random = &n1; // 2's random -> 1
    n3.random = &n2; // 3's random -> 2

    NodeWithRandom *clone = cloneListWithRandom(&n1);
    printf("Clone head val: %d, random: %d\n", clone->val, clone->random->val);
    printf("Clone n2 val:   %d, random: %d\n", clone->next->val, clone->next->random->val);
    return 0;
}
*/


/* ============================================================================
   SECTION 7: STACKS, QUEUES, SEARCHING & SORTING (Q45 - Q50)
   ============================================================================ */

/* ----------------------------------------------------------------------------
   Q45. Balanced Parentheses Validator Using an Array Stack
   Difficulty: Easy-Medium | Time: O(N), Space: O(N)
   Why Asked: Tests fundamental stack LIFO logic for '()', '{}', '[]'.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool isParenthesesBalanced(const char *expr) {
    if (!expr) return true;

    int len = strlen(expr);
    char stack[len + 1];
    int top = -1;

    for (int i = 0; expr[i] != '\0'; i++) {
        char ch = expr[i];

        if (ch == '(' || ch == '{' || ch == '[') {
            stack[++top] = ch; // Push
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (top == -1) return false; // Stack underflow

            char open = stack[top--]; // Pop
            if ((ch == ')' && open != '(') ||
                (ch == '}' && open != '{') ||
                (ch == ']' && open != '[')) {
                return false; // Mismatched pair
            }
        }
    }

    return (top == -1); // Must be empty
}

int main() {
    printf("{[()]} balanced? %s\n", isParenthesesBalanced("{[()]}") ? "YES" : "NO");
    printf("{[(])} balanced? %s\n", isParenthesesBalanced("{[(])}") ? "YES" : "NO");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q46. Next Greater Element for Every Element (Monotonic Stack)
   Difficulty: Medium | Time: O(N), Space: O(N)
   Example: [4, 5, 2, 25] -> [5, 25, 25, -1]
   Why Asked: Classic monotonic stack problem avoiding the naive O(N^2) search.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

void nextGreaterElement(const int arr[], int n, int result[]) {
    if (!arr || !result || n <= 0) return;

    int stack[n];
    int top = -1;

    // Traverse from right to left
    for (int i = n - 1; i >= 0; i--) {
        // Pop elements smaller than or equal to current element
        while (top != -1 && stack[top] <= arr[i]) {
            top--;
        }

        // Top of stack is next greater element
        result[i] = (top == -1) ? -1 : stack[top];

        // Push current element
        stack[++top] = arr[i];
    }
}

int main() {
    int arr[] = {4, 5, 2, 25};
    int n = sizeof(arr) / sizeof(arr[0]);
    int nge[4];
    nextGreaterElement(arr, n, nge);
    printf("Next Greater Elements: ");
    for (int i = 0; i < n; i++) printf("%d ", nge[i]); // 5 25 25 -1
    printf("\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q47. Min Stack Implementation in Pure C (All Operations in O(1))
   Difficulty: Medium | Time: O(1) for push, pop, top, getMin, Space: O(N)
   Why Asked: Tests tracking minimum state alongside primary data using a dual-stack
              or encoded values.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define STACK_CAPACITY 100

typedef struct {
    int data[STACK_CAPACITY];
    int min_data[STACK_CAPACITY];
    int top;
} MinStack;

void minStackInit(MinStack *s) {
    s->top = -1;
}

void minStackPush(MinStack *s, int val) {
    if (s->top >= STACK_CAPACITY - 1) return; // Overflow

    s->top++;
    s->data[s->top] = val;

    if (s->top == 0) {
        s->min_data[s->top] = val;
    } else {
        int current_min = s->min_data[s->top - 1];
        s->min_data[s->top] = (val < current_min) ? val : current_min;
    }
}

int minStackPop(MinStack *s) {
    if (s->top < 0) return INT_MIN; // Underflow
    return s->data[s->top--];
}

int minStackTop(MinStack *s) {
    if (s->top < 0) return INT_MIN;
    return s->data[s->top];
}

int minStackGetMin(MinStack *s) {
    if (s->top < 0) return INT_MIN;
    return s->min_data[s->top];
}

int main() {
    MinStack s;
    minStackInit(&s);
    minStackPush(&s, 10);
    minStackPush(&s, 20);
    minStackPush(&s, 5);
    minStackPush(&s, 8);

    printf("Current Min: %d\n", minStackGetMin(&s)); // 5
    minStackPop(&s); // Pop 8
    minStackPop(&s); // Pop 5
    printf("Min after popping 5: %d\n", minStackGetMin(&s)); // 10
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q48. Evaluate Postfix Expression (Reverse Polish Notation)
   Difficulty: Medium | Time: O(N), Space: O(N)
   Example: "2 3 1 * + 9 -" -> -4
   Why Asked: Fundamental stack application for expression evaluation.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int evaluatePostfix(const char *expr) {
    if (!expr) return 0;

    int stack[128];
    int top = -1;

    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == ' ' || expr[i] == '\t') continue;

        if (isdigit(expr[i])) {
            int num = 0;
            while (isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            i--; // Step back for outer loop
            stack[++top] = num;
        } else {
            // Operator: pop two operands
            if (top < 1) return 0; // Malformed expression
            int val2 = stack[top--];
            int val1 = stack[top--];

            switch (expr[i]) {
                case '+': stack[++top] = val1 + val2; break;
                case '-': stack[++top] = val1 - val2; break;
                case '*': stack[++top] = val1 * val2; break;
                case '/': stack[++top] = (val2 != 0) ? (val1 / val2) : 0; break;
            }
        }
    }

    return (top >= 0) ? stack[top] : 0;
}

int main() {
    const char *postfix = "10 2 8 * + 3 -"; // (10 + 2*8) - 3 = 26 - 3 = 23
    printf("Result: %d\n", evaluatePostfix(postfix)); // 23
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q49. In-Place QuickSort in Pure C
   Difficulty: Medium | Time: O(N log N) avg, O(N^2) worst, Space: O(log N)
   Why Asked: Tests recursion, partitioning logic, and pointer swapping.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

static void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Lomuto partition scheme with last element as pivot
static int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    int arr[] = {10, 7, 8, 9, 1, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    quickSort(arr, 0, n - 1);
    printf("Sorted array: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   Q50. QuickSelect (Find Kth Smallest / Largest Element in Average O(N))
   Difficulty: Medium-Hard | Time: O(N) average, Space: O(1) iterative
   Why Asked: Standard interview question when full sorting (O(N log N)) is inefficient.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

static void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static int partition(int arr[], int l, int r) {
    int pivot = arr[r];
    int i = l - 1;
    for (int j = l; j < r; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[r]);
    return i + 1;
}

int quickSelectKthSmallest(int arr[], int n, int k) {
    // k is 1-based index (1 <= k <= n)
    int l = 0, r = n - 1;
    int target_idx = k - 1;

    while (l <= r) {
        int p = partition(arr, l, r);
        if (p == target_idx) {
            return arr[p];
        } else if (p < target_idx) {
            l = p + 1;
        } else {
            r = p - 1;
        }
    }

    return -1;
}

int main() {
    int arr[] = {12, 3, 5, 7, 4, 19, 26};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 3; // 3rd smallest should be 5 (sorted: 3, 4, 5, 7, 12, 19, 26)
    printf("%drd smallest element: %d\n", k, quickSelectKthSmallest(arr, n, k));
    return 0;
}
*/




/* ============================================================================
   CODING QUESTIONS TRANSFERRED FROM _NOTES.MD (VERIFIED, CORRECTED & EXPANDED)
   ============================================================================ */

/* ----------------------------------------------------------------------------
   NQ1. Add and Subtract Two Integers Without '+' or '-' Operators
   Source: _notes.md
   Explanation:
   - Addition: carry is (x & y) << 1, sum without carry is x ^ y.
   - Subtraction: borrow is ((~x) & y) << 1, difference without borrow is x ^ y.
                  Alternatively, use two's complement: x + (~y) + 1.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

int add_bitwise(int x, int y) {
    while (y != 0) {
        int carry = x & y;
        x = x ^ y;
        y = carry << 1;
    }
    return x;
}

int subtract_bitwise(int x, int y) {
    while (y != 0) {
        int borrow = (~x) & y;
        x = x ^ y;
        y = borrow << 1;
    }
    return x;
}

int subtract_twos_complement(int x, int y) {
    return add_bitwise(x, add_bitwise(~y, 1));
}

int main() {
    printf("Add (15 + 27): %d\n", add_bitwise(15, 27));                       // 42
    printf("Sub (42 - 15): %d\n", subtract_bitwise(42, 15));                  // 27
    printf("Sub TwosComp (50 - 20): %d\n", subtract_twos_complement(50, 20)); // 30
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ2. Custom sizeof Operator Implementation Using Macros
   Source: _notes.md
   Explanation:
   - For a type: pointer arithmetic on NULL pointer ((size_t)((TYPE *)0 + 1))
   - For a variable: pointer difference between (&var + 1) and (&var) cast to char*
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stddef.h>

#define SIZEOF_TYPE(T) ((size_t)((T *)0 + 1))
#define SIZEOF_VAR(v)  ((size_t)((char *)(&(v) + 1) - (char *)(&(v))))

int main() {
    int x = 10;
    double d = 3.14;
    char arr[25];

    printf("sizeof(int) via type:    %zu\n", SIZEOF_TYPE(int));       // 4
    printf("sizeof(double) via type: %zu\n", SIZEOF_TYPE(double));    // 8
    printf("sizeof(x) via var:       %zu\n", SIZEOF_VAR(x));          // 4
    printf("sizeof(d) via var:       %zu\n", SIZEOF_VAR(d));          // 8
    printf("sizeof(arr) via var:     %zu\n", SIZEOF_VAR(arr));        // 25
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ3. Delete a Specific Bit from a Number and Shift Higher Bits Down
   Source: _notes.md
   Explanation:
   To delete bit at position 'pos' and collapse the number:
   - High part: shift right by (pos + 1), then shift left by pos
   - Low part: mask lowest pos bits using ((1U << pos) - 1)
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

unsigned int deleteBit(unsigned int num, int pos) {
    if (pos < 0 || pos >= 32) return num;
    unsigned int left = num >> (pos + 1);
    unsigned int right = num & ((1U << pos) - 1);
    return (left << pos) | right;
}

int main() {
    // 0b10110 (22): delete bit 2 (0-indexed) -> 0b1010 (10)
    unsigned int n = 22;
    printf("Before: %u (0x%X), After deleting bit 2: %u (0x%X)\n",
           n, n, deleteBit(n, 2), deleteBit(n, 2));
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ4. Swap 5th and 7th Nibbles in a 32-bit Word & Arbitrary Nibble Swapper
   Source: _notes.md
   Explanation:
   In 32-bit unsigned int:
   Nibble 0: bits 0-3, Nibble 4: bits 16-19, Nibble 6: bits 24-27.
   To swap nibbles at arbitrary positions n1 and n2 (0-7):
   extract both, mask out original positions, and reinsert swapped.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>

uint32_t swapNibblesArbitrary(uint32_t val, int n1, int n2) {
    if (n1 == n2 || n1 < 0 || n1 > 7 || n2 < 0 || n2 > 7) return val;

    uint32_t val1 = (val >> (n1 * 4)) & 0x0F;
    uint32_t val2 = (val >> (n2 * 4)) & 0x0F;

    // Clear original nibble slots
    val &= ~((0x0FU << (n1 * 4)) | (0x0FU << (n2 * 4)));

    // Place swapped values
    val |= (val1 << (n2 * 4)) | (val2 << (n1 * 4));
    return val;
}

int main() {
    // Swap 4th and 6th nibble (0-indexed, corresponding to 5th and 7th 1-indexed)
    uint32_t var = 0x11223344;
    uint32_t res = swapNibblesArbitrary(var, 4, 6);
    printf("Original: 0x%08X, Swapped: 0x%08X\n", var, res);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ5. Reverse Byte Order in a 32-bit Integer
   Source: _notes.md
   Explanation:
   Converts Little-Endian to Big-Endian or vice versa:
   0x12345678 -> 0x78563412.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>

uint32_t reverseBytes32(uint32_t num) {
    return ((num & 0xFF000000) >> 24) |
           ((num & 0x00FF0000) >> 8)  |
           ((num & 0x0000FF00) << 8)  |
           ((num & 0x000000FF) << 24);
}

// Full byte + nibble reversal (0x12345678 -> 0x78654321)
uint32_t reverseBytesAndNibbles(uint32_t num) {
    uint32_t b_rev = reverseBytes32(num);
    // Swap nibbles in each byte
    return ((b_rev & 0xF0F0F0F0) >> 4) | ((b_rev & 0x0F0F0F0F) << 4);
}

int main() {
    uint32_t val = 0x12345678;
    printf("Byte reversed:            0x%08X\n", reverseBytes32(val));          // 0x78563412
    printf("Byte and nibble reversed: 0x%08X\n", reverseBytesAndNibbles(val));   // 0x87654321
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ6. Binary String Addition
   Source: _notes.md
   Explanation:
   Given two binary strings (e.g. "11101" and "01011"), return their sum
   in binary ("101000") handling unequal string lengths and carry.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <string.h>

char* addBinaryStrings(const char *s1, const char *s2, char *result, size_t max_len) {
    if (!s1 || !s2 || !result || max_len == 0) return NULL;

    int i = strlen(s1) - 1;
    int j = strlen(s2) - 1;
    int carry = 0;
    size_t idx = 0;

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += s1[i--] - '0';
        if (j >= 0) sum += s2[j--] - '0';

        if (idx >= max_len - 1) break; // Buffer protection
        result[idx++] = (sum % 2) + '0';
        carry = sum / 2;
    }
    result[idx] = '\0';

    // Reverse generated string to correct order
    for (size_t x = 0, y = idx - 1; x < y; x++, y--) {
        char temp = result[x];
        result[x] = result[y];
        result[y] = temp;
    }

    return result;
}

int main() {
    char res[64];
    addBinaryStrings("11101", "01011", res, sizeof(res));
    printf("11101 + 01011 = %s\n", res); // 101000
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ7. Software Button Debounce Algorithm
   Source: _notes.md
   Explanation:
   Prevents mechanical switch bounce from registering multiple false edges by
   requiring a minimum delay (e.g. 50ms) between consecutive triggers.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <time.h>

#define DEBOUNCE_DELAY_MS 50

int is_button_press_debounced(clock_t *last_time) {
    clock_t now = clock();
    double elapsed_ms = ((double)(now - *last_time) / CLOCKS_PER_SEC) * 1000.0;

    if (elapsed_ms >= DEBOUNCE_DELAY_MS) {
        *last_time = now;
        return 1; // Valid debounced press accepted
    }
    return 0; // Ignored as contact bounce
}

int main() {
    clock_t last_trigger = 0;
    printf("Event 1: %s\n", is_button_press_debounced(&last_trigger) ? "ACCEPTED" : "IGNORED");
    printf("Event 2 (immediate): %s\n", is_button_press_debounced(&last_trigger) ? "ACCEPTED" : "IGNORED");
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ8. Standard C String Function Implementations (strlen, strcpy, strcmp, strcat)
   Source: _notes.md
   Explanation:
   Core string implementations written without <string.h> helpers.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stddef.h>

size_t custom_strlen(const char *str) {
    if (!str) return 0;
    const char *s = str;
    while (*s) s++;
    return (size_t)(s - str);
}

char* custom_strcpy(char *dest, const char *src) {
    if (!dest || !src) return dest;
    char *orig = dest;
    while ((*dest++ = *src++) != '\0');
    return orig;
}

int custom_strcmp(const char *s1, const char *s2) {
    if (!s1 && !s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;

    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

char* custom_strcat(char *dest, const char *src) {
    if (!dest || !src) return dest;
    char *orig = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++) != '\0');
    return orig;
}

int main() {
    char buf[64] = "Hello";
    custom_strcat(buf, " World");
    printf("Concat: %s, Len: %zu\n", buf, custom_strlen(buf));
    printf("Cmp: %d\n", custom_strcmp("abc", "abd")); // Negative
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ9. Format and Print 32-bit Integer in Binary Representation
   Source: _notes.md
   Explanation:
   Inspects each bit from MSB (31) to LSB (0) with space grouping every 8 bits.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

void printBinaryFormatted(unsigned int n) {
    printf("0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 8 == 0 && i != 0) printf(" ");
    }
    printf("\n");
}

int main() {
    printBinaryFormatted(0x12345678);
    printBinaryFormatted(255);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ10. String Reversal Using Three Distinct Techniques
   Source: _notes.md
   Explanation:
   Method 1: Two-pointer swap
   Method 2: Recursive range swap
   Method 3: In-place XOR swap (no temporary variable)
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <string.h>

void reverseStringTwoPointer(char *str) {
    if (!str) return;
    int l = 0, r = strlen(str) - 1;
    while (l < r) {
        char tmp = str[l]; str[l] = str[r]; str[r] = tmp;
        l++; r--;
    }
}

void reverseStringRecursive(char *str, int l, int r) {
    if (!str || l >= r) return;
    char tmp = str[l]; str[l] = str[r]; str[r] = tmp;
    reverseStringRecursive(str, l + 1, r - 1);
}

void reverseStringXOR(char *str) {
    if (!str) return;
    int l = 0, r = strlen(str) - 1;
    while (l < r) {
        str[l] ^= str[r];
        str[r] ^= str[l];
        str[l] ^= str[r];
        l++; r--;
    }
}

int main() {
    char s1[] = "Hello";
    char s2[] = "World";
    char s3[] = "Embedded";

    reverseStringTwoPointer(s1);
    reverseStringRecursive(s2, 0, strlen(s2) - 1);
    reverseStringXOR(s3);

    printf("s1: %s, s2: %s, s3: %s\n", s1, s2, s3);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ11. Check if Two Strings are Anagrams Using Frequency Array
   Source: _notes.md
   Explanation:
   Single 256-element integer frequency table. Increments for s1, decrements for s2.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <string.h>

int areAnagrams(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;
    if (strlen(s1) != strlen(s2)) return 0;

    int count[256] = {0};

    for (int i = 0; s1[i] != '\0'; i++) {
        count[(unsigned char)s1[i]]++;
        count[(unsigned char)s2[i]]--;
    }

    for (int i = 0; i < 256; i++) {
        if (count[i] != 0) return 0;
    }
    return 1;
}

int main() {
    printf("listen vs silent: %d\n", areAnagrams("listen", "silent")); // 1
    printf("apple vs pale:     %d\n", areAnagrams("apple", "pale"));     // 0
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ12. In-Place Duplicate Character Removal from String Using Visited Table
   Source: _notes.md
   Explanation:
   Maintains relative character ordering while eliminating repeats in O(N) time.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

void removeDuplicateCharsOrder(char *str) {
    if (!str) return;

    int seen[256] = {0};
    int write_idx = 0;

    for (int read_idx = 0; str[read_idx] != '\0'; read_idx++) {
        unsigned char ch = (unsigned char)str[read_idx];
        if (!seen[ch]) {
            seen[ch] = 1;
            str[write_idx++] = str[read_idx];
        }
    }
    str[write_idx] = '\0';
}

int main() {
    char s[] = "programming";
    removeDuplicateCharsOrder(s);
    printf("Unique chars: %s\n", s); // "progami"
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ13. Custom realloc Implementation
   Source: _notes.md
   Explanation:
   Reallocates memory to new_size, copies minimum of (old_size, new_size), and frees original.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* custom_realloc(void *ptr, size_t old_size, size_t new_size) {
    if (!ptr) return malloc(new_size);
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    void *new_ptr = malloc(new_size);
    if (!new_ptr) return NULL;

    size_t copy_size = (old_size < new_size) ? old_size : new_size;
    memcpy(new_ptr, ptr, copy_size);
    free(ptr);

    return new_ptr;
}

int main() {
    int *arr = (int *)malloc(3 * sizeof(int));
    arr[0] = 10; arr[1] = 20; arr[2] = 30;

    arr = (int *)custom_realloc(arr, 3 * sizeof(int), 5 * sizeof(int));
    arr[3] = 40; arr[4] = 50;

    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");
    free(arr);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ14. Fixed-Size Memory Pool Allocator for Embedded Systems
   Source: _notes.md
   Explanation:
   Pre-allocates static buffer blocks. O(1) allocation and free without heap fragmentation.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define POOL_BLOCK_COUNT 8
#define POOL_BLOCK_SIZE  64

typedef struct {
    uint8_t memory[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
    uint8_t used[POOL_BLOCK_COUNT];
} StaticMemPool;

void pool_init(StaticMemPool *p) {
    memset(p->used, 0, sizeof(p->used));
}

void* pool_alloc(StaticMemPool *p) {
    if (!p) return NULL;
    for (int i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!p->used[i]) {
            p->used[i] = 1;
            return (void *)p->memory[i];
        }
    }
    return NULL; // Pool exhausted
}

void pool_free(StaticMemPool *p, void *ptr) {
    if (!p || !ptr) return;
    for (int i = 0; i < POOL_BLOCK_COUNT; i++) {
        if ((void *)p->memory[i] == ptr) {
            p->used[i] = 0;
            return;
        }
    }
}

int main() {
    StaticMemPool pool;
    pool_init(&pool);

    void *b1 = pool_alloc(&pool);
    void *b2 = pool_alloc(&pool);
    printf("Allocated blocks: %p, %p\n", b1, b2);

    pool_free(&pool, b1);
    void *b3 = pool_alloc(&pool);
    printf("Re-allocated block b3 (should match b1): %p\n", b3);
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ15. Circular / Ring Buffer FIFO (Embedded Essential)
   Source: _notes.md
   Explanation:
   Fixed-size circular buffer using power-of-2 size and bitwise mask for O(1) FIFO queue.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>
#include <stdint.h>

#define CBUF_SIZE 8 // Power of 2 required

typedef struct {
    uint8_t buffer[CBUF_SIZE];
    volatile int head; // Write index
    volatile int tail; // Read index
} CircularBuffer;

void cb_init(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
}

int cb_is_full(const CircularBuffer *cb) {
    return ((cb->head + 1) & (CBUF_SIZE - 1)) == cb->tail;
}

int cb_is_empty(const CircularBuffer *cb) {
    return cb->head == cb->tail;
}

int cb_put(CircularBuffer *cb, uint8_t data) {
    if (cb_is_full(cb)) return -1; // Overflow
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) & (CBUF_SIZE - 1);
    return 0;
}

int cb_get(CircularBuffer *cb, uint8_t *data) {
    if (cb_is_empty(cb)) return -1; // Underflow
    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) & (CBUF_SIZE - 1);
    return 0;
}

int main() {
    CircularBuffer q;
    cb_init(&q);

    cb_put(&q, 10);
    cb_put(&q, 20);
    cb_put(&q, 30);

    uint8_t val;
    while (!cb_is_empty(&q)) {
        cb_get(&q, &val);
        printf("Dequeued: %d\n", val);
    }
    return 0;
}
*/

/* ----------------------------------------------------------------------------
   NQ16. Finite State Machine (FSM) via Function Pointer Matrix
   Source: _notes.md
   Explanation:
   Clean, decoupled event-driven state machine pattern for embedded systems.
   ---------------------------------------------------------------------------- */
/*
#include <stdio.h>

typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_COUNT
} FsmState;

typedef enum {
    EVENT_START,
    EVENT_STOP,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} FsmEvent;

typedef FsmState (*FsmHandler)(FsmEvent event);

FsmState handle_idle(FsmEvent event) {
    switch (event) {
        case EVENT_START: printf("IDLE -> RUNNING\n"); return STATE_RUNNING;
        case EVENT_ERROR: printf("IDLE -> ERROR\n");   return STATE_ERROR;
        default: return STATE_IDLE;
    }
}

FsmState handle_running(FsmEvent event) {
    switch (event) {
        case EVENT_STOP:  printf("RUNNING -> IDLE\n");  return STATE_IDLE;
        case EVENT_ERROR: printf("RUNNING -> ERROR\n"); return STATE_ERROR;
        default: return STATE_RUNNING;
    }
}

FsmState handle_error(FsmEvent event) {
    if (event == EVENT_RESET) {
        printf("ERROR -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

FsmHandler fsm_table[STATE_COUNT] = {
    handle_idle,
    handle_running,
    handle_error
};

int main() {
    FsmState current = STATE_IDLE;
    current = fsm_table[current](EVENT_START);
    current = fsm_table[current](EVENT_STOP);
    current = fsm_table[current](EVENT_ERROR);
    current = fsm_table[current](EVENT_RESET);
    return 0;
}
*/
