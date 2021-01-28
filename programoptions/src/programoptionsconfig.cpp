#include <fstream>
#include "iostream"
#include "programoptionsconfig.hpp"

#include <boost/program_options.hpp>
using namespace boost;
using namespace std;
namespace po = boost::program_options;

BoostConfig::BoostConfig( const int argc, const char* const argv[] )
{
    //declare help messages, only accessible by command line
    string config_file = "../config.conf";
    po::options_description generic("General options");
    generic.add_options()
            ("help,h", "display help message");
    //("config", po::value<string>(&config_file)->default_value("../config.conf"), "set config file using './simpleheat @filename', default value = ../config.conf");

    //declare and present the supported options, accessible by command line or by configuration file
    std::vector<int> buff_gs, buff_de;
    std::vector<double> buff_ds;
    po::options_description config("Program options");
    config.add_options()
            ("max_iteration", po::value<int>(&m_nb_iter), "set maximal iteration")
            ("global_shape", po::value< vector<int> >(&buff_gs)->multitoken(), "set the shape of global data field")
            ("dist_extents", po::value< std::vector<int> >(&buff_de)->multitoken(), "set the shape of data distribution")
            ("delta_t", po::value<double>(&m_delta_t), "set the time difference between two consecutive points")
            ("delta_s", po::value< std::vector<double> >(&buff_ds)->multitoken(), "set the space difference between two consecutive points");

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config);

    po::options_description config_file_options;
    config_file_options.add(config);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    //add parameters to the map to be stored
    po::variables_map vm;

    //print help messages
    if (vm.count("help"))
    {
        cout<<generic<<endl;
        cout<<config<<endl;
    }
    else
    {
        ifstream ifs(config_file.c_str());
        if (ifs) {
            po::store(po::parse_config_file(ifs, config), vm);
            po::notify(vm);
        }
    }

/** Code for MIXED input from a config file and command line is not yet supported in this version
 * @param argc the number of command-line arguments
 * @param argv the values of command-line arguments
 * the function of MIXED input from a config file and command line is not yet supported in this version
 */
 /*
    //code abandoned
    std::ifstream settings_file("../config.conf");
    po::store(po::parse_config_file(settings_file, config), vm);
    po::notify(vm);

    po::options_description all("All options");
    all.add(generic).add(config);

    if (vm.count("config"))
    {
        string conf_name = vm["config"].as<string>();
        ifstream ifs_config(conf_name.c_str());

        if (! ifs_config)
        {
            cerr << "could not open the configure file" << endl;
        }

        stringstream ss_config;
        ss_config << ifs_config.rdbuf();
    }
    po::notify(vm);*/

    //write data into type coor2d
    m_global_shape[DX] = buff_gs[0];
    m_global_shape[DY] = buff_gs[1];
    m_dist_extents[DX] = buff_de[0];
    m_dist_extents[DY] = buff_de[1];
    m_delta_space[0] = buff_ds[0];
    m_delta_space[1] = buff_ds[1];

    //print out parameters
    cout<<"max_iteration: "<<m_nb_iter<<endl;
    cout<<"global_shape: "<<m_global_shape[DX]<<" "<<m_global_shape[DY]<<endl;
    cout<<"dist_extents: "<<m_dist_extents[DX]<<" "<<m_dist_extents[DY]<<endl;
    cout<<"delta_t: "<<m_delta_t<<endl;
    cout<<"delta_s: "<<m_delta_space[DX]<<" "<<m_delta_space[DY]<<endl;
}

