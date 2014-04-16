
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_graph.h"

static zend_function_entry graph_functions[] = {
	PHP_FE(random_graph, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry graph_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_GRAPH_EXTNAME,
	graph_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
#if ZEND_MODULE_API_NO >=20010901
	PHP_GRAPH_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_GRAPH
ZEND_GET_MODULE(graph)
#endif


//implementation of a custom my_function

PHP_FUNCTION(random_graph) // some of the output information should be visulized on webpage
{

//two variables in graph, Number of Nodes, Number of edges
//they should be input values from the screen
#define Nnodes 10  
#define Nedges 30

//-----------------------------------------------------------------
//creating graph edge stored in adjacency matrax M[Nnodes][Nnodes]

        int i,j,k,num;
        int M[Nnodes][Nnodes];
        
        for(i=0;i<Nnodes;i++)
        {
                for(j=0;j<=i;j++)
                {
                        M[i][j]=M[j][i]=0;
                }
        }
        
        php_printf("\n---randomly creating edges:---\n");
        
        k=0;
        while(k<Nedges)
        {
                i= rand() % Nnodes;
                j= rand() % Nnodes;
                if(i!=j&&M[i][j]!=1)
                {
                //      M[i][j]=M[i][i]=1; //treated as undirected graph
                        M[i][j]=1;       // treated as directed graph
                        k=k+1;
                }
        }

        php_printf("\n---printing adjacency matrix:---\n");
        
        num=0;
        for(i=0;i<Nnodes;i++)
        {
                for(j=0;j<Nnodes;j++)
                {
                        php_printf("%d ",M[i][j]);
                        if(M[i][j]==1)
                        {
                                num=num+1;
                        }
                }
                php_printf("\n");
        }

//----------------------------------------------------------------      
//degree of verticies stored in degree[Nnodes]

        int indegree[Nnodes];
        int outdegree[Nnodes];
        int degree[Nnodes];

        php_printf("\n---calculating degree of nodes:---\n");
        
        for(i=0;i<Nnodes;i++)
        {
                indegree[i]=outdegree[i]=degree[i]=0;
        }
        
        for(i=0;i<Nnodes;i++)
        {
                for(j=0;j<Nnodes;j++)
                {
                        if(M[i][j]==1)
                        {
                                indegree[j]+=1;
                                outdegree[i]+=1;
                                degree[i]+=1;
                                degree[j]+=1;
                        }
                }
        }

        for(i=0;i<Nnodes;i++)
        {
                php_printf("node %d : indegree %d outdegree %d total-degree %d\n", i, indegree[i], outdegree[i], degree[i]);
        }
        

//-------------------------------------------------------
//calculating pagerank of each node stored in pgrk[Nnodes]

        
#define d 0.85
#define TOLL 1e-5

        double pgrk[Nnodes],b[Nnodes];
        int pgi,pgj,pgk,pgl,pgt;      //in this part, some variables have prefix pg-
        double diff=1.00;             //loop self-consistance end mark

        pgk=pgl=pgt=0;

        php_printf("\n---calculating pagerank---\n");

//set initial value
        for(pgi=0;pgi<Nnodes;pgi++)
        {
                pgrk[pgi]=1.00/Nnodes;
                b[pgi]=1.00/Nnodes;
        }

        while(diff>=TOLL)  // if convergence
        {
                for(pgi=0;pgi<Nnodes;pgi++)
                {
                        b[pgi]=(1-d)/Nnodes;
                        for(pgj=0;pgj<Nnodes;pgj++)
                        {       
                                if(M[pgj][pgi]==1) // a directed edge j --> i 
                                {
                                        pgl=0;
                                        for(pgk=0;pgk<Nnodes;pgk++)
                                        {
                                                if(M[pgj][pgk]==1)
                                                        pgl=pgl+1;   //outdegree of pgj
                                        }
                                        b[pgi]=b[pgi]+d*pgrk[pgj]/pgl;
                                }
                        }
                }
                diff=0.00;
                for(pgi=0;pgi<Nnodes;pgi++)
                {
                        diff=diff+fabsf(pgrk[pgi]-b[pgi]); // absolute value of difference
                }
                for(pgi=0;pgi<Nnodes;pgi++)  // copy the new pagerank information b[] into pgrk[]
                {
                        pgrk[pgi]=b[pgi];
                }
        }
        
        for(pgi=0;pgi<Nnodes;pgi++)
        {
                php_printf("Pagerank of node %d : %f\n", pgi,pgrk[pgi]);
        }


//--------------------------------------------------
//geodesics calculation stored in gd[Nnodes][Nnodes]
//use Floyd-Warshall algorithm

#define MAXGD 10000  //maximum possible geodesics in the graph

        int gd[Nnodes][Nnodes];
        int next[Nnodes][Nnodes];
        int gdi,gdj,gdk;
        int MAXPATH;

        php_printf("\n---calculating geodesics---\n");

//initialing
        for(gdi=0;gdi<Nnodes;gdi++)
        {
                for(gdj=0;gdj<Nnodes;gdj++)
                {
                        next[gdi][gdj]=0;
                        if(M[gdi][gdj]!=0)
                        {
                                gd[gdi][gdj]=M[gdi][gdj];
                        }
                        else
                        {
                                gd[gdi][gdj]=MAXGD;
                        }
                }
        }

//Floyd-Warshall algorithm to calculate geodesics
        for(gdk=0;gdk<Nnodes;gdk++)
        {
                for(gdi=0;gdi<Nnodes;gdi++)
                {
                        for(gdj=0;gdj<Nnodes;gdj++)
                        {
                                if(gd[gdi][gdk]+gd[gdk][gdj]<gd[gdi][gdj])
                                {
                                        gd[gdi][gdj]=gd[gdi][gdk]+gd[gdk][gdj];
                                        next[gdi][gdj]=k;  //the largest nodenumber that is included in geodesics of i->j
                                }
                        }
                }
        }

        MAXPATH=0;
        php_printf("geodesic path length, from node i to node j...:\n");
        for(gdi=0;gdi<Nnodes;gdi++)
        {
                for(gdj=0;gdj<Nnodes;gdj++)
                {
                        if(gd[gdi][gdj]<MAXGD)
                        {
                                if(MAXPATH<gd[gdi][gdj])
                                {
                                        MAXPATH=gd[gdi][gdj]; //longest geodesic path length
                                }
                                php_printf("%d      ", gd[gdi][gdj]);
                        }
                        else
                        {
                                php_printf("NULL    ");
                        }
                }
                php_printf("\n");
        }

        php_printf("\n---calculating path histogram---\n");
        php_printf("Maximum path of this graph = %d\n", MAXPATH);

//creating geodesic histogram stored in gdh[MAXPATH]
        int gdh[MAXPATH];
        
        for(gdi=0;gdi<MAXPATH;gdi++)
        {
                gdh[gdi]=0;
        }

        for(gdi=0;gdi<Nnodes;gdi++)
        {
                for(gdj=0;gdj<Nnodes;gdj++)
                {
                        if(gd[gdi][gdj]<MAXGD)
                        {
                                gdh[gd[gdi][gdj]-1]++;
                        }
                }
        }

        for(gdi=0;gdi<MAXPATH;gdi++)
        {
                php_printf("path length %d, number: %d\n", gdi+1, gdh[gdi]);
        }

//neighborhood information from geodesics matrix gd[Nnodes][Nnodes]

        int tempgd;
        int r,s,t;
        
        php_printf("\n---calculate neighborhood of each node---\n");

        for(r=0;r<Nnodes;r++)
        {
                php_printf("Node %d:", r);

                tempgd=0;
                for(s=0;s<Nnodes;s++)       //getting all the neighborhood
                {
                        if(gd[r][s]!=MAXGD && tempgd<gd[r][s])
                                tempgd=gd[r][s];   // largest geodesics of node r
                }
                tempgd=2;         //getting neighborhood within distance 2
                for(t=1;t<=tempgd;t++)
                {
                        for(s=0;s<Nnodes;s++)
                        {
                                if(gd[r][s]==t)
                                        php_printf(" %d", s);
                        }
                }
                php_printf("\n");
        }
        
        RETURN_TRUE;
}


