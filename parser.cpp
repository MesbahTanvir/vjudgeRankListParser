#include <bits/stdc++.h>
using namespace std;
#define MAXN 1005
vector < string > vec;
char data[MAXN];
string team[MAXN];
int totalPenalty[MAXN];
int totalSolve[MAXN];
bool acTab[MAXN][MAXN]; // totalTeam x totalProblem

void removeLeadingTrailingSpaces(string &str){
    while(str.empty()==false and str.back()==' ') str.pop_back();
    reverse(str.begin(),str.end());
    while(str.empty()==false and str.back()==' ') str.pop_back();
    reverse(str.begin(),str.end());
}
int parseRank(string str){
    //<td class="rank meta">1</td>
    int start=false;
    string num;
    for(auto c : str){
        if(c=='>') start=true;
        if(start and c>='0' and c<='9') num+=c;
        if(c=='<' and start) break;
    }
    removeLeadingTrailingSpaces(num);
    return atoi(num.c_str());
}
string parseTeamName(string str){
    //<div><a href="/user/zuccbot_exe" target="_blank">zuccbot_exe <span style="color:gray">(TonmoyNirjhorSafayat)</span></a></div>
    str = str.substr(5,str.size()-string("</span></a></div>").size() - string("<div>").size());
    //<a href="/user/zuccbot_exe" target="_blank">zuccbot_exe <span style="color:gray">(TonmoyNirjhorSafayat)
    string name;
    int start=false;
    for(auto c : str){
        if(c=='<' and start) break;
        if(start) name+=c;
        if(c=='>') start=true;
    }
    removeLeadingTrailingSpaces(name);
    return name;
}
int parsePenalty(string str){
    //<td class="penalty meta">   <span class="minute">1319</span>   <span class="hms">21:59:49</span></td>
    int cnt=0;
    string num;
    int start =false;
    for(auto c : str){
        if(cnt==2) start=true;
        if(c=='<' and start ) break;
        if(c>='0' and c<='9') num+=c;
        if(c=='>') cnt++;
    }
    removeLeadingTrailingSpaces(num);
    return atoi(num.c_str());
}
bool parseProblemStatus(string str){
    //<td class="prob accepted">00:14:20<br>&nbsp;</td>
    for(int i =0 ; i < str.size() ; i++){
        if(str.substr(i,string("accepted").size())=="accepted") return 1;
    }
    return 0;
}

void removeSpaces(){
    for(auto &str : vec){
        removeLeadingTrailingSpaces(str);
    }
}
void parse(){

    removeSpaces();
    memset(totalSolve,0,sizeof(totalSolve));
    int rowNumber=0, colNumber=0;
    int teamRank=0,problemNumber=0;
    string teamName="";
    for(auto str : vec){
        if(str.substr(0,4)=="<tb "){
            // first line of input stream
        }
        if(str.substr(0,4)=="<tr "){
            // new row started
            // no significant information on this line

            rowNumber++;
            colNumber=1;
            // reseting data
            teamRank =0;
            problemNumber=0;
            teamName ="";
        }
        if(str.substr(0,4)=="<td "){
            if(colNumber==1){
                // first col contain rank
                teamRank = parseRank(str);
                colNumber++;
            }
            else if(colNumber==2){
                // no significant information
                colNumber++;
            }
            else if(colNumber==5){
                // no significant information
                colNumber++;
            }
            else if(colNumber==6){
                int penalty = parsePenalty(str);
                totalPenalty[teamRank]=penalty;
                colNumber++;
            }
            else {
                for(int i=7; ; i++){
                    if(colNumber==i){
                        problemNumber = (i-6);
                        bool problemStatus = parseProblemStatus(str);
                        assert(teamRank);
                        assert(problemNumber);
                        acTab[teamRank][problemNumber]=problemStatus;
                        if(problemStatus) totalSolve[teamRank]++;
                        colNumber++;
                        break;
                    }
                    if(i==50) break;

                }
                // there should no more than 50 problem
            }
        }
        if(str.substr(0,4)=="<div"){
            assert(colNumber==3);
            teamName = parseTeamName(str);
            team[teamRank]=teamName;
            colNumber++;
        }
        if(str.substr(0,4)=="</td"){
            // no significant information
            assert(colNumber==4);
            colNumber++;
        }
        if(str.substr(0,4)=="</tr"){
            // row end
        }
    }
    cout << rowNumber <<" "<<colNumber-7<<endl;
    for(int i=1;i<=rowNumber;i++){
        cout << team[i] <<" "<<totalSolve[i]<<" "<<totalPenalty[i]<<endl;
    }
    for(int i=1;i<=rowNumber;i++){
        for(int j=1;j<=colNumber-7;j++){
            cout << acTab[i][j]<<" ";
        }
        cout <<endl;
    }

}

int main(int argc , char *args[])
{
    char output[100],input[100];
    bool fromConsole=true;
    if(fromConsole){
        int a; cin >>a;
        sprintf(output,"contest%02d.txt",a);
        cin >> input;
    }
    else {
        int a = atoi(args[1]);
        sprintf(output,"contest%02d.txt",a);
        sprintf(input,"%s",args[2]);
    }
    freopen(input,"r",stdin);
    freopen(output,"w",stdout);
    while(gets(data)){
        vec.push_back(data);
    }
    parse();
    return 0;
}
