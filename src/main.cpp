#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
using namespace std;

const int screenWidth = 1000;
const int screenHeight = 750;

const int arrSize = 50;
const int rectSize = 20;

vector<int> arr(arrSize);
vector<int> newArr(arrSize);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool complete = false;

bool init()
{
    bool success = true;
    
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout<<"Unable to initialize SDL: "<<SDL_GetError()<<"\n";
        success = false;
    }
    else
    {
        window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
        if(window == NULL)
        {
            cout<<"Could not create window: "<<SDL_GetError();
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED);
            if(renderer == NULL)
            {
                cout<<"Could not create renderer: "<<SDL_GetError();
                success = false;
            }
        }
    }  
    return success;
}

void close()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void visualize(int x = -1, int y = -1, int z = -1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j = 0;
    for(int i = 0; i <= screenWidth - rectSize; i += rectSize)
    {
        SDL_PumpEvents();

        SDL_Rect rect = {i, screenHeight, rectSize, -arr[j]};

        if(complete)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if(j == x)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(j == y)
        {
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(j == z)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

void heapify(int n, int i)
{
    int maxIdx = i;
    int l = 2*i+1;
    int r = 2*i+2;

    if(l < n && arr[l] > arr[maxIdx])
    {
        maxIdx = l;
    }

    if(r < n && arr[r] > arr[maxIdx])
    {
        maxIdx = r;
    }

    if(maxIdx != i)
    {
        swap(arr[i], arr[maxIdx]);

        visualize(i,maxIdx);
        SDL_Delay(30);

        heapify(n, maxIdx);
    }
}

void heapSort()
{
    for(int i = (arrSize/2) - 1; i >= 0; i--)
    {
        heapify(arrSize, i);
    }

    for(int i=arrSize-1; i>0; i--)
    {
        swap(arr[0], arr[i]);

        visualize(0,i);
        SDL_Delay(30);

        heapify(i,0);
    }
}

int partition(int start, int end)
{
    int pivot = arr[end];
    int j = start - 1;
    for(int i = start; i < end; i++)
    {
        if(arr[i] <= pivot)
        {
            j++;
            swap(arr[j], arr[i]);

            visualize(end, i, j);
            SDL_Delay(30);
        }
    }  
    swap(arr[j+1], arr[end]);

    visualize(j+1, end);
    SDL_Delay(30);

    return j+1;
}

void quickSort(int start, int end)
{
    if(start >= end)
    {
        return;
    }
    int pi = partition(start, end);
    quickSort(start, pi - 1);
    quickSort(pi + 1, end);
}

void merge2SortedArrays(int start, int mid, int end)
{
    vector<int> output(end - start + 1);
    int i = start;
    int j = mid + 1;
    int k = 0;
    while (i <= mid && j <= end)
    {
        if (arr[i] < arr[j])
        {
            output[k] = arr[i];

            visualize(i, j);
            SDL_Delay(30);

            k++;
            i++;
        }
        else
        {
            output[k] = arr[j];

            visualize(i, j);
            SDL_Delay(30);

            k++;
            j++;
        }
    }
    while (i <= mid)
    {
        output[k] = arr[i];

        visualize(-1, i);
        SDL_Delay(30);

        k++;
        i++;
    }
    while (j <= end)
    {
        output[k] = arr[j];

        visualize(-1, j);
        SDL_Delay(30);

        k++;
        j++;
    }

    k = 0;
    for(int l = start; l <= end; l++)
    {
        arr[l] = output[k];

        visualize(l);
        SDL_Delay(30);

        k++;
    }
}

void mergeSort(int start, int end)
{
    if(start >= end)
    {
        return;
    }
    int mid = (start + end)/2;

    mergeSort(start, mid);
    mergeSort(mid+1, end);

    merge2SortedArrays(start, mid, end);
}

void insertionSort()
{
    for(int i = 1; i < arrSize; i++)
    {
        int j = i - 1;
        int temp = arr[i];
        while(j >= 0 && arr[j] > temp)
        {
            arr[j+1]=arr[j];
            j--;

            visualize(i, j+1,j);
            SDL_Delay(30);
        }
        arr[j+1]=temp;
    }
}

void bubbleSort()
{
    for(int i = 0; i < arrSize-1; i++)
    {
        for(int j = 0; j<arrSize-1-i; j++)
        {
            if(arr[j+1] < arr[j])
            {
                swap(arr[j+1],arr[j]);

                visualize(arrSize-i, j+1, j);
                SDL_Delay(30);
            }
        }
    }
}

void selectionSort()
{
    int minIndex;
    for(int i = 0; i < arrSize - 1; i++)
    {
        minIndex = i;
        for(int j = i + 1; j < arrSize; j++)
        {
            if(arr[j] < arr[minIndex])
            {
                minIndex = j;

                visualize(i, minIndex);
                SDL_Delay(30);
            }
        }
        swap(arr[i], arr[minIndex]);

        visualize(i, minIndex);
        SDL_Delay(3);
    }
}

void loadArr()
{
    arr = newArr;
}

void randomizeArray()
{
    srand(time(NULL));
    for(int i = 0; i < arrSize; i++)
    {
        newArr[i] = rand()%(screenHeight);
    }
}

void execute()
{
    if(!init())
    {
        cout<<"SDL Initialization Failed.\n";
    }
    else
    {
        randomizeArray();
        loadArr();

        SDL_Event event;
        bool quit = false;

        while(!quit)
        {
            while(SDL_PollEvent(&event) != 0)
            {

                if(event.type == SDL_QUIT)
                {
                    quit = true;
                    complete = false;
                }
                else if(event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                        case (SDLK_0):
                            randomizeArray();
                            complete = false;
                            loadArr();
                            cout<<"\nNEW RANDOM LIST GENERATED.\n";
                            break;
                        case SDLK_1:
                            loadArr();
                            cout<<"\nSELECTION SORT STARTED.\n";
                            complete = false;
                            selectionSort();
                            complete = true;
                            cout<<"\nSELECTION SORT COMPLETED\n";
                            break;
                        case SDLK_2:
                            loadArr();
                            cout<<"\nBUBBLE SORT STARTED.\n";
                            complete = false;
                            bubbleSort();
                            complete = true;
                            cout<<"\nBUBBLE SORT COMPLETED\n";
                            break;
                        case SDLK_3:
                            loadArr();
                            cout<<"\nINSERTION SORT STARTED.\n";
                            complete = false;
                            insertionSort();
                            complete = true;
                            cout<<"\nINSERTION SORT COMPLETED\n";
                            break;
                        case SDLK_4:
                            loadArr();
                            cout<<"\nMERGE SORT STARTED.\n";
                            complete = false;
                            mergeSort(0, arrSize - 1);
                            complete = true;
                            cout<<"\nMERGE SORT COMPLETED\n";
                            break;
                        case SDLK_5:
                            loadArr();
                            cout<<"\nQUICK SORT STARTED.\n";
                            complete = false;
                            quickSort(0, arrSize - 1);
                            complete = true;
                            cout<<"\nQUICK SORT COMPLETED\n";
                            break;
                        case SDLK_6:
                            loadArr();
                            cout<<"\nHEAP SORT STARTED.\n";
                            complete = false;
                            heapSort();
                            complete = true;
                            cout<<"\nHEAP SORT COMPLETED\n";
                            break;
                        case SDLK_q:
                            quit = true;
                            complete = false;
                            cout<<"\nEXITING SORTING VISUALIZER\n";
                            break;
                    }
                }
            }
            visualize();
        }
        close();
    }
}

bool controls()
{
    cout<<"\n\n*AVAILABLE CONTROLS INSIDE SORTING VISUALIZER:*\n"
        <<"    Use 0 to generate random list.\n"
        <<"    Use 1 to start 'SELECTION SORT ALGORITHM'.\n"
        <<"    Use 2 to start 'BUBBLE SORT ALGORITHM'.\n"
        <<"    Use 3 to start 'INSERTION SORT ALGORITHM'.\n"
        <<"    Use 4 to start 'MERGE SORT ALGORITHM'.\n"
        <<"    Use 5 to start 'QUICK SORT ALGORITHM'.\n"
        <<"    Use 6 to start 'HEAP SORT ALGORITHM'.\n"
        <<"    Use q to exit 'SORTING VISUALIZER'.\n\n"
        <<"WARNING: Giving repetitive commands may cause latency and the visualizer may behave unexpectedly. Please give a new command only after the current command's execution is done.\n\n"
        <<"Press ENTER to start 'SORTING VISUALIZER'.\n"
        <<"OR type 0 and press ENTER to exit the PROGRAM --> ";

    string s;
    getline(cin, s);

    if(s == "0")
    {
        return false;
    }

    return true;
}

void intro()
{
    cout<<"\n\n\n\n\t\t\t\t|SORTING VISUALIZER|\n\n"
        <<"\n\nVisualization of different sorting algorithams in C++ with SDL2 library\n"
        <<"Sorting algorithms are used to sort a data structure according to a specific order relationship, such as numerical order or lexicographical order.\n"
        <<"Sorting algorithms can be difficult to understand and it's easy to get confused. We believe visualizing sorting algorithms can be a great way to better understand their functioning while having fun!\n"
        <<"The sorting algorithms covered here are Selection Sort, Insertion Sort, Bubble Sort, Merge Sort, Quick Sort and Heap Sort.\n"
        <<"Press ENTER to show controls --> ";
    
    string s;
    getline(cin, s);

    if(s == "\n")
    {
        return;
    }
}

int main(int argc, char* args[])
{
    intro();

    while(true)
    {
        if(controls())
        {
            execute();
        }
        else
        {
            cout<<"\n\nEXITING PROGRAM.\n";
            break;
        }
    }
    
    return 0;
}