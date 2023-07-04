#include<iostream>
#include<atomic>
#include<pthread.h>
#include<ctime>
#include<fstream>
#include<unistd.h>
#include<random>
#include<chrono>

using namespace std;

struct ipdata{

    int index;
    int k;
    double l1;
    double l2;
    int * avg_waiting_time;
    int * max_waiting_time;
};



typedef struct ipdata* Ipdata;

atomic_flag lock_stream = ATOMIC_FLAG_INIT;

string ordinal_suffix_of(int i) {

    int j = i % 10,
        k = i % 100;
    if (j == 1 && k != 11) {
        return "st";
    }
    if (j == 2 && k != 12) {
        return "nd";
    }
    if (j == 3 && k != 13) {
        return "rd";
    }
    return "th";
}

void * t_func(void * t_pass){

    Ipdata data = (Ipdata)t_pass;

    int i = data->index;
    int k = data->k;
    double l1 = data->l1;
    double l2 = data->l2;
    int * avg_waiting_time = data->avg_waiting_time;
    int * max_waiting_time = data->max_waiting_time;

    for(int p = 0; p < k; p++){

        ofstream ofile;
        ofile.open("output_tas.txt", ios::app);

        struct timespec ts0;
        timespec_get(&ts0, TIME_UTC);

        time_t tmNow0;
        tmNow0 = time(NULL);
        struct tm t0 = *localtime(&tmNow0);

        ofile<<p+1<<ordinal_suffix_of(p+1)<<" CS Request at "<<t0.tm_hour<<":"<<t0.tm_min<<":"<<t0.tm_sec<<":"<<ts0.tv_nsec/1000000<<" by thread"<<i+1<<endl;

        ofile.close();

        while(lock_stream.test_and_set());

        ofile.open("output_tas.txt", ios::app);

        struct timespec ts1;
        timespec_get(&ts1, TIME_UTC);

        time_t tmNow1;
        tmNow1 = time(NULL);
        struct tm t1 = *localtime(&tmNow1);

        int current_waiting_time = (t1.tm_hour-t0.tm_hour)*3600000 + (t1.tm_min - t0.tm_min)*60000 + (t1.tm_sec - t0.tm_sec)*1000 + (ts1.tv_nsec- ts0.tv_nsec)/1000000;
        avg_waiting_time[i] = avg_waiting_time[i]*p + current_waiting_time;
        avg_waiting_time[i] /= (p+1);
        if(current_waiting_time > max_waiting_time[i]){

            max_waiting_time[i] = current_waiting_time;
        }

        ofile<<p+1<<ordinal_suffix_of(p+1)<<" CS Entry at "<<t1.tm_hour<<":"<<t1.tm_min<<":"<<t1.tm_sec<<":"<<ts1.tv_nsec/1000000<<" by thread"<<i+1<<endl;

        unsigned seed1 =
            std::chrono::system_clock::now().time_since_epoch().count();

        std::default_random_engine generator1 (seed1);
        std::exponential_distribution<double> exponential1((double)1000/l1);

        double st1 = exponential1(generator1);

        sleep(st1);

        lock_stream.clear();

        struct timespec ts2;
        timespec_get(&ts2, TIME_UTC);

        time_t tmNow2;
        tmNow2 = time(NULL);
        struct tm t2 = *localtime(&tmNow2);

        ofile<<p+1<<ordinal_suffix_of(p+1)<<" CS Exit at "<<t2.tm_hour<<":"<<t2.tm_min<<":"<<t2.tm_sec<<":"<<ts2.tv_nsec/1000000<<" by thread"<<i+1<<endl;

        ofile.close();

        unsigned seed2 =
            std::chrono::system_clock::now().time_since_epoch().count();

        std::default_random_engine generator2 (seed2);
        std::exponential_distribution<double> exponential2((double)1000/l2);

        double st2 = exponential2(generator2);

        sleep(st2);
    }

    free(t_pass);

    return NULL;
}

int main(int argc, char *argv[]){

    ifstream ipfile;

    ipfile.open(argv[1]);

    int N,k;    
    double l1,l2;

    ipfile>>N>>k>>l1>>l2;

    pthread_t thread[N];
    int *avg_waiting_time = (int *)malloc(sizeof(int)*N);
    int *max_waiting_time = (int *)malloc(sizeof(int)*N);

    for(int i = 0; i < N; i++){

        avg_waiting_time[i] = 0;
        max_waiting_time[i] = 0;
    }

    ofstream ofile;
    ofile.open("output_tas.txt");
    ofile<<"TAS ME Output\n\n";
    ofile.close();

    for(int i = 0; i < N; i++){

        Ipdata a = (Ipdata)malloc(sizeof(struct ipdata));
        
        a->index = i;
        a->k = k;
        a->l1 = l1;
        a->l2 = l2;
        a->avg_waiting_time = avg_waiting_time;
        a->max_waiting_time = max_waiting_time;

        pthread_create(&thread[i], NULL, &t_func, (void *)a);
    }

    for(int i = 0; i < N; i++){

        pthread_join(thread[i], NULL);
    }

    int average_wait_time = 0;
    int max_wait_time = 0;

    for(int i = 0; i < N; i++){

        average_wait_time += avg_waiting_time[i]; 

        if(max_waiting_time[i] > max_wait_time){

            max_wait_time = max_waiting_time[i];
        }
    }

    average_wait_time /= N;

    cout<<"Avg: "<<average_wait_time <<" Max: "<< max_wait_time<<endl;

    return 0;
}