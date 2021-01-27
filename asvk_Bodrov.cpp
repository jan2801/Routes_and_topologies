#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>               //file work
#include <cstring>
#include <vector>
#include <cmath>
#include <time.h>   //clock()


#define  R 6371                //the Earth's radius

using namespace std;

int find_node(int id);


vector <class node> nodes;
vector <class edge> edges;
int maxn=0;
int maxe=0;
ofstream fout_w;



struct node
{
    int id;
    char *label;
    //char *country;
    double longitude;
    int internal;
    double latitude;
    bool hyperedge;
    node(int a, char *name, int c, double b=0,  double d=0)
    {
        id=a;
        label=name;
        longitude=b;
        internal=c;
        latitude=d;
        hyperedge=false;
    }
};

struct edge
{
    int source;
    int target;
    double distance;         //in km
    double delay;           //delay in milliseconds
    //  char *LinkLabel;
    edge(int i, int g)
    {
        source=i;
        target=g;
        distance=0;       //the distance between two nodes
        delay=0;
    
    }
    void dist_and_delay()
    {
        int num1=find_node(source);
        int num2=find_node(target);
        //if neither first, no second node have a hyperedge, we can count distance and delay
        if ((!nodes[num1].hyperedge)&&(!nodes[num2].hyperedge))
        {
            double d=sin(nodes[num1].latitude*0.015708)*sin(nodes[num2].latitude*0.015708)+cos(nodes[num1].latitude*0.015708)*cos(nodes[num2].latitude*0.015708)*cos(nodes[num1].longitude*0.015708-nodes[num2].longitude*0.015708);
            d=acos(d);
            distance=d*R;
            delay=distance*0.0048;
        }
            
        /*
        cos(d)=sin(φА)·sin(φB)+cos(φА)·cos(φB)·cos(λА − λB),
        where φА - latitude of a source
        φB - latitude of a target
        λА - longtitude of a source
        λB - longtitude of a target

        1 grad = 0,015708 rad

        L=d·R
        where L is a distance in km, R - the Earth's average radius in km and 
        d is a distance between points, measured in radians by the arc of a large circle of the globe

        */
    }

};


double** ways_creator(double **way)
{
    //create ways like it is posssible to move as from source to target, as from target to source
    int i, j, k;
    for (k=0; k<maxn; k++)
        for (i=0; i<maxn; i++)
            way[k][i]=0; //that means that these two nodes haven't way between them
    for (k=0; k<maxn; k++)
        for (i=0; i<maxn; i++)
            for (j=0; j<maxe; j++)
                    if ((edges[j].source==k)&&(edges[j].target==i))
                    {
                        way[k][i]=edges[j].delay;
                        way[i][k]=edges[j].delay;
                    }
    return way;
}




int find_node(int id)
{
    int k;
    for (k=0; k<maxn; k++)
    {
        if (nodes[k].id==id)
        {
            return k;
        }
    }

    cout << endl << "there isn't node with id № " << id;
    perror("error");

    exit(1);
}




char *copystr(char *tmp)
{
    int i=0;
    int j=4;
    char *str=(char *) calloc(j, sizeof(char));
    while (*(tmp+i) != '\0')
        {
            if (i+1==j)
            {
                j*=2;
                str=(char *) realloc(str, j*sizeof(char));
            }
            *(str+i)=*(tmp+i);
            i++;
        }
        *(str+i)='\0';
    return str;
}



char *readstr(void)
{
    int bufsize=150;
    int position=0;
    char *buffer=(char *) calloc(sizeof(char), bufsize);
    int c;

    if (!buffer) 
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    while (1) 
    {
        c=getchar();
        if (c == EOF || c == '\n' || c == '\0') 
        {
            buffer[position]='\0';
            return buffer;
        } 
        else 
        {
            buffer[position]=c;
        }
        position++;
        if (position>=bufsize) 
        {
            bufsize+=10;
            buffer=(char *) realloc(buffer, bufsize);
            if (!buffer) 
            {
                fprintf(stderr, "Memory error\n");

                exit(1);
            }
        }
    }
    return buffer;
}




void parsing(char *name)
{
    clock_t start, stop;
    start=clock();
    int id;
    int hyper_flag=0;
    int target;
    int end_flag=2;
    int source;
    double longitude;
    int internal;
    double latitude;
    int i=0;
    char *label=(char *) calloc(40, sizeof(char));
    char *buf=(char *) calloc(1000, sizeof(char));
    ifstream fin (name);
    fin >> buf;
    while (end_flag)
    {
        stop=clock();
        if ((stop-start)/CLOCKS_PER_SEC>10)
        {
            perror("inccorrect input");
            exit(1);
        }
        while ((strcmp(buf, "node") != 0) && (strcmp(buf, "edge") != 0) && (end_flag))     //posible to use fin.end() instead of end_flag
        {
            stop=clock();
            if ((stop-start)/CLOCKS_PER_SEC>10)
            {
                perror("inccorrect input");
                exit(1);
            }
            if (strcmp(buf, "]") == 0)
                end_flag--;
            else
            {
                end_flag=2;
            }
            fin >> buf;
        }


        if (strcmp(buf, "node") == 0)
        {
            end_flag=2;
            maxn++;
            while (strcmp(buf, "id") != 0)
            {
                fin >> buf;
                stop=clock();
                if ((stop-start)/CLOCKS_PER_SEC>10)
                {
                    perror("inccorrect input");
                    exit(1);
                }
            }
            fin >> buf;
            id=atoi(buf);
            while ((strcmp(buf, "label") != 0))
            {
                fin >> buf;
                stop=clock();
                if ((stop-start)/CLOCKS_PER_SEC>10)
                {
                    perror("inccorrect input");
                    exit(1);
                }

            }
            fin >> buf;
        
            label=copystr(buf);

            while ((strcmp(buf, "hyperedge") != 0) && (strcmp(buf, "Internal") != 0) && (strcmp(buf, "Country") != 0))
            {
                stop=clock();
                if ((stop-start)/CLOCKS_PER_SEC>10)
                {
                    perror("inccorrect input");
                    exit(1);
                }
                fin >> buf;
                if ((strcmp(buf, "hyperedge") != 0) && (strcmp(buf, "Internal") != 0) && (strcmp(buf, "Country") != 0))
                {

                    label=strcat(label, " ");
                    label=strcat(label, buf);
                }
            }
            int short_data=0;
            if ((strcmp(buf, "hyperedge") == 0) || (strcmp(buf, "Internal") == 0))
            {
                //hyperedge is a kind of abstraction, so we will not count edges with them like edges in creating the shortest way
                hyper_flag=1;
                if (strcmp(buf, "Internal") == 0)
                {
                    short_data=1;
                }
                fin >> buf;
            }
            

            else while ((strcmp(buf, "Longitude") != 0))
            {
                stop=clock();
                if ((stop-start)/CLOCKS_PER_SEC>10)
                {
                    perror("inccorrect input");
                    exit(1);
                }
                fin >> buf;
            }
            
            if (!hyper_flag)
            {
                if (strcmp(buf, "Longitude") == 0)
                    fin >> buf;
                longitude=atof(buf);
            
            }
            if (!short_data)
                while (strcmp(buf, "Internal") != 0)
                {
                    fin >> buf;
                    stop=clock();
                    if ((stop-start)/CLOCKS_PER_SEC>10)
                    {
                        perror("inccorrect input");
                        exit(1);
                    }

                }
            if (!short_data)
                fin >> buf;
            internal=atoi(buf);
            while ((strcmp(buf, "Latitude") != 0)&&(strcmp(buf, "]") != 0))
                fin >> buf;
           // if (strcmp(buf, "]") != 0)
            fin >> buf;
            //if (strcmp(buf, "Latitude") == 0)
            latitude=atof(buf); 
            if (!hyper_flag)
            {
                node new_node(id, label, internal, longitude, latitude);
                nodes.insert(nodes.end(), new_node);   //we create our vector of nodes here
            }
            if (hyper_flag)
            {
                node new_node(id, label, internal);
                new_node.hyperedge=true;
                nodes.insert(nodes.end(), new_node);   //we create our vector of nodes here
            }  
            hyper_flag=0;                     
            //what to do if we are on edge
        }
    
        else if (strcmp(buf, "edge") == 0)
        {
            end_flag=2;
            maxe++;
            while (strcmp(buf, "source") != 0)
                fin >> buf;
            fin >> buf;
            source=atoi(buf);
            while (strcmp(buf, "target") != 0)
                fin >> buf;
            fin >> buf;
            target=atoi(buf); 
            
            edge new_edge(source, target);
            
            edges.insert(edges.end(), new_edge);    //we create our vector of edges here
            
            //what to do if we are on node
        } 
        
    }
    if (buf)
        free(buf);
    fin.close();  
}





void sh_way(int i, int j, int **next_node)
{
    if (next_node[i][j] != j)
    {
        
        fout_w << next_node[i][j] << " "; 
        sh_way(next_node[i][j], j, next_node);
        
    }
    else
    {
        fout_w << j << " "; 
    
    }
}




void sh_way_consol(int i, int j, int **next_node)
{
    if (next_node[i][j] != j)
    {
        
        cout << next_node[i][j] << " "; 
        sh_way_consol(next_node[i][j], j, next_node);
        
    }
    else
    {
        cout << j << " "; 
    
    }
}







int main(int argc, char **argv)
{
    //use clock for 10 seconds to catch some kind of mistakes, if topology is really big, we need to give more time
    clock_t start, stop;
    start=clock ();
    int i, j, k, m, p;
    char *name=(char *) calloc(25, sizeof(char));
    char *name1=(char *) calloc(25, sizeof(char));
    char *str=(char *) calloc(25, sizeof(char));
    if (argc<3)
    {
        perror("not enough input data\n");
        exit(1);
    }
    for (p=0; p<argc; p++)
    {
        
        if (!strcmp(argv[p], "-t"))
        {
            str=strcpy(str, argv[p+1]);
            if ((str[strlen(str)-1] != 'l') || (str[strlen(str)-2] != 'm') || (str[strlen(str)-3] != 'g') || (str[strlen(str)-4] != '.'))
            {
                cout << str[strlen(str)-1];
                perror("format of topology is not correct");
                exit(1);
            }
            parsing(argv[p+1]);
            name=strncpy(name, argv[p+1], strlen(argv[p+1])-4);
            double** way=new double*[maxn];
            int** next_node=new int*[maxn];
            for (i=0; i<maxn; i++)
            {
                way[i]=new double[maxn];
                next_node[i]=new int[maxn];
            }
            for (j=0; j<maxe; j++)
            {
                edges[j].dist_and_delay();
            }
            ofstream fout;
            
            name=strcat(name, "_topo.csv");
            fout.open(name);           
            fout << "Node 1 (id)" << "," << "Node 1 (label)" << "," << "Node 1 (longitude)" << "," << "Node 1 (latitude)" << ",";
            fout << "Node 2 (id)" << "," << "Node 2 (label)" << "," << "Node 2 (longitude)" << "," << "Node 2 (latitude)" << ",";
            fout << "Distance (km)" << "," << "Delay (ms)" << endl;

            for (k=0; k<maxe; k++)
            {
                for (j=0; j<maxn; j++)
                {
                    for (m=0; m<maxn; m++)
                    {
                        if ((edges[k].source == nodes[j].id)&&(edges[k].target == nodes[m].id))
                        {
                            //here we write edges to file with edges
                            fout << nodes[j].id << "," << nodes[j].label << "," << nodes[j].longitude << "," << nodes[j].latitude << ",";
                                
                            fout << nodes[m].id << "," << nodes[m].label << "," << nodes[m].longitude << "," << nodes[m].latitude << ",";
                        
                            fout << edges[k].distance << "," << edges[k].delay << endl;
                        }
                    }
                        
                    
                }
            } 
        
            fout.close();

            
            way=ways_creator(way);
            for (i=0; i<maxn; i++)
                for (j=0; j<maxn; j++)
                    next_node[i][j]=j;
            for (k=0; k<maxn; k++)
                for (i=0; i<maxn; i++)
                    for (j=0; j<maxn; j++)
                        if (((way[i][j] == 0) || (way[i][k]+way[k][j] < way[i][j])) && (way[i][k] != 0) && (way[k][j] != 0))            //Floyd–Warshall algorithm
                        {
                            //here we create the shortest ways with Floyd–Warshall algorithm
                            way[i][j]=way[i][k]+way[k][j];
                            next_node[i][j]=k;
                            way[j][i]=way[i][j];
                            next_node[j][i]=k;
                            
                        }
            
            name1=strncpy(name1, name, strlen(name)-9);
            name1=strcat(name1, "_routes.csv");
            fout_w.open(name1);
            fout_w << "Node 1 (id)" << "," << "Node 2 (id)" << "," << "Path type" << "," << "Path" << "," << "Delay (ms)" << endl;
            for (i=0; i<maxn; i++)
                for (j=0; j<maxn; j++)
                {
                    if (way[i][j])
                    {
                        //here we print in file the shortest ways with Floyd–Warshall algorithm
                        if (i != j)
                        {
                            fout_w << i << "," << j << "," << "Main" << "," << "[ " << i <<" ";
                            sh_way(i, j, next_node);
                            fout_w << "]" << "," << way[i][j] << endl;
                        }
                    }
                }
            if (argc>6) 
                if (!strcmp(argv[p+2], "-s"))
                {
                    
                    if (strcmp(argv[p+4], "-d"))
                    {
                        perror("incorrect input data\n");
                        exit(1);
                    }
                    i=atoi(argv[p+3]);
                    j=atoi(argv[p+5]);
                    if ((i > maxn) || (j > maxn))
                    {
                        perror("incorrect node's number\n");
                        exit(1);
                    }
        
                    if (way[i][j] != 0)
                    {
                        //here we print the path of the way between two nodes
                        cout << i << " ";
                        sh_way_consol(i, j, next_node);
                    }
                    else
                    {
                        printf("these nodes haven't way between them or the way have hyperedges or short-data nodes, so it is impossible to count the shortest way");
                    }
                    
                }


       
            if (strcmp(argv[p], "-r"))
            {
                //delete the node of the main way, then try to find the shortest way in new graf
            }
            if (name)
            {
                free(name);
            }
            for (i=0; i<maxn; i++)
            {
                delete[] way[i];
                delete[ ] next_node[i];
            }
            delete[] way;
            delete[] next_node;
        }
    }

 

    fout_w.close();
    stop=clock();
    return 0;
}
