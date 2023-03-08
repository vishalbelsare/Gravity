//
//  DCOPF.cpp
//  Gravity
//
//  Created by Hassan Hijazi on 19 Jan 18.
//
//
#include <stdio.h>
#include <PowerNet.h>
#include <gravity/solver.h>
#ifdef USE_OPT_PARSER
#include <optionParser.hpp>
#endif
#include <gravity/rapidcsv.h>
#include "Roster.hpp"
using namespace std;
using namespace gravity;

int main (int argc, char * argv[])
{
    string fname = string(prj_dir)+"/data_sets/Roster/Enrollment_Details_23.csv";
    
    
#ifdef USE_OPT_PARSER
    /** Create a OptionParser with options */
    auto options = readOptions(argc, argv);
    options.add_option("f", "file", "Input file name", fname);
    
    /** Parse the options and verify that all went well. If not, errors and help will be shown */
    bool correct_parsing = options.parse_options(argc, argv);
    
    if(!correct_parsing){
        return EXIT_FAILURE;
    }
    
    fname = options["f"];
    bool has_help = op::str2bool(options["h"]);
    if(has_help) {
        options.show_help();
        exit(0);
    }
#else
    if(argc>=2){
        fname=argv[1];
    }
#endif
    rapidcsv::Document doc(fname);
    map<string, vector<Team>> LA_divs, WR_divs;
    auto nb_players = doc.GetRowCount();
    cout << "There are " <<  nb_players << " registered players." << std::endl;
    vector<int> ages = doc.GetColumn<int>("Player Age");
    vector<string> fnames = doc.GetColumn<string>("Player First Name");
    vector<string> lnames = doc.GetColumn<string>("Player Last Name");
    vector<string> genders = doc.GetColumn<string>("Player Gender");
    vector<string> emails_vec = doc.GetColumn<string>("User Email");
    vector<string> loc_full = doc.GetColumn<string>(doc.GetColumnCount()-2);
    vector<string> loc_season = doc.GetColumn<string>(doc.GetColumnCount()-3);
    set<string> emails;
    int min_team_size = 8, max_team_size = 16;
    for(auto i = 0; i<nb_players; i++){
        int age = ages[i];
        Player p(fnames[i], lnames[i], ages[i], emails_vec[i], genders[i]);
        emails.insert(emails_vec[i]);
        if(loc_full[i]=="Los Alamos" || loc_season[i]=="Los Alamos"){
            string div_name = "U"+to_string(age+1);
            string team_name = "LA_"+div_name;
            if(LA_divs.count(div_name)==0)
                LA_divs[div_name].push_back(Team(team_name));
            LA_divs[div_name].back().players.push_back(p);
        }
        else if(loc_full[i]=="White Rock" || loc_season[i]=="White Rock"){
            string div_name = "U"+to_string(age+1);
            string team_name = "WR_"+div_name;
            if(WR_divs.count(div_name)==0)
                WR_divs[div_name].push_back(Team(team_name));
            WR_divs[div_name].back().players.push_back(p);
        }
        else
            cout << "WARNING: preferred location unspecified, not sure where to place player! Row number: " << i << endl;
    }
    DebugOn("There are " << LA_divs.size() << " divisions in LA " << endl);
    DebugOn("There are " << WR_divs.size() << " divisions in WR " << endl);
    /* First, combining small teams */
    
    bool min_size_ok = false;
    while(!min_size_ok){
        map<string, vector<Team>> new_LA_divs = LA_divs;
        for(auto const &div: LA_divs){
            DebugOn("LA Division " << div.first);
            Team t =  div.second.back();
            DebugOn(" has " << t.players.size() << " players" << endl);
            if(t.players.size()<min_team_size){
                DebugOn("Merging teams to satisfy min size constraint" << endl);
                string div_name = div.first;
                int div_num = stoi(div_name.substr(div_name.find_first_of("U")+1));
                string div_up_name = "U"+to_string(div_num+1);
                auto it = LA_divs.find(div_up_name);
                if(it!=LA_divs.end()){
                    Team new_team = t.combine_with(it->second.back());
                    new_LA_divs[div_name+"_"+div_up_name].push_back(new_team);
                    new_LA_divs.erase(div_name);
                    new_LA_divs.erase(div_up_name);
                }
                else{
                    string div_down_name = "U"+to_string(div_num-1);
                    auto it = LA_divs.find(div_down_name);
                    Team new_team = t.combine_with(it->second.back());
                    new_LA_divs[div_down_name+"_"+div_name].push_back(new_team);
                    new_LA_divs.erase(div_name);
                    new_LA_divs.erase(div_down_name);
                }
                break;
            }
        }
        LA_divs = new_LA_divs;
        min_size_ok = true;
        for(auto const &div: LA_divs){
            Team t =  div.second.back();
            if(t.players.size()<min_team_size){
                min_size_ok = false;
            }
        }
    }
    min_size_ok = false;
    while(!min_size_ok){
        map<string, vector<Team>> new_WR_divs = WR_divs;
        for(auto const &div: WR_divs){
            DebugOn("WR Division " << div.first);
            Team t =  div.second.back();
            DebugOn(" has " << t.players.size() << " players" << endl);
            if(t.players.size()<min_team_size){
                DebugOn("Merging teams to satisfy min size constraint" << endl);
                string div_name = div.first;
                int div_num = stoi(div_name.substr(div_name.find_first_of("U")+1));
                string div_up_name = "U"+to_string(div_num+1);
                auto it = WR_divs.find(div_up_name);
                if(it!=WR_divs.end()){
                    Team new_team = t.combine_with(it->second.back());
                    new_WR_divs[div_name+"_"+div_up_name].push_back(new_team);
                    new_WR_divs.erase(div_name);
                    new_WR_divs.erase(div_up_name);
                }
                else{
                    string div_down_name = "U"+to_string(div_num-1);
                    auto it = WR_divs.find(div_down_name);
                    Team new_team = t.combine_with(it->second.back());
                    new_WR_divs[div_down_name+"_"+div_name].push_back(new_team);
                    new_WR_divs.erase(div_name);
                    new_WR_divs.erase(div_down_name);

                }
                break;
            }
        }
        WR_divs = new_WR_divs;
        min_size_ok = true;
        for(auto const &div: WR_divs){
            Team t =  div.second.back();
            if(t.players.size()<min_team_size){
                min_size_ok = false;
            }
        }
    }
    
    for(auto const &div: LA_divs){
        DebugOn("LA Division " << div.first);
        for(Team t: div.second){
            DebugOn(" has " << t.players.size() << " players" << endl);
            if(t.players.size()>max_team_size){
                DebugOn("Splitting team to satisfy max size constraint" << endl);
                int nb_factors = ceil(t.players.size()/(double)max_team_size);
                vector<Team> split_t = t.split_into(nb_factors);
                LA_divs[div.first] = split_t;
            }
        }
    }
    for(auto const &div: WR_divs){
        DebugOn("WR Division " << div.first);
        for(Team t: div.second){
            DebugOn(" has " << t.players.size() << " players" << endl);
            if(t.players.size()>max_team_size){
                DebugOn("Splitting team to satisfy max size constraint" << endl);
                int nb_factors = ceil(t.players.size()/(double)max_team_size);
                vector<Team> split_t = t.split_into(nb_factors);
                WR_divs[div.first] = split_t;
            }
        }
    }
    for(auto const &div: LA_divs){
        DebugOn("\n\nLA Division " << div.first << endl);
        for(auto &t: div.second){
            DebugOn("\n\tLA Team " << t.team_name << endl);
            DebugOn("\t " << t.players.size() << " players" << endl);
            for(auto const &p: t.players){
                DebugOn("\t\t" << p.first_name << " " << p.last_name  << endl);
            }
            DebugOn("Email list for team:\n");
            for(auto const &p: t.players){
                DebugOn(p.email << ";");
            }
        }
    }
    for(auto const &div: WR_divs){
        DebugOn("\n\nWR Division " << div.first << endl);
        for(auto const &t: div.second){
            DebugOn("\n\tWR Team " << t.team_name << endl);
            DebugOn("\t " << t.players.size() << " players" << endl);
            for(auto const &p: t.players){
                DebugOn("\t\t" << p.first_name << " " << p.last_name  << endl);
            }
            DebugOn("Email list for team:\n");
            for(auto const &p: t.players){
                DebugOn(p.email << ";");
            }
        }
    }
    bool print_emails = true;
    if(print_emails){
        DebugOn("\n\nPrinting full email lists in batches of 100 recipients or less:" <<endl);
        int idx = 1;
        for(const auto &em: emails){
            if(idx%100==0)
                DebugOn("\nNEW BATCH\n");
            DebugOn(em<<";");
            idx++;
        }
    }
    DebugOn("\nDone parsing enrollment file" <<endl);
    return 0;
}
