#include "argparse.h"

void get_opts(int argc,
              char **argv,
              struct options_t *opts)
{
    if (argc == 1)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "\t--num_cluster or -k <num_cluster>" << std::endl;
        std::cout << "\t--dims or -d <dim>" << std::endl;
        std::cout << "\t--inputfilename or -i <inputfilename>" << std::endl;
        // std::cout << "\t--outputfilename or -o <inputfilename>" << std::endl;
        std::cout << "\t--max_num_iter or -m <max_num_iter>" << std::endl;
        std::cout << "\t--threshold or -t <threshold>" << std::endl;
        std::cout << "\t--c or -c <output>" << std::endl;
        std::cout << "\t--seed or -s <seed>" << std::endl;
        std::cout << "\t--imp or -a <implementation>" << std::endl;
        exit(0);
    }

    opts->c = false;

    struct option l_opts[] = {
        {"num_cluster", required_argument, NULL, 'k'},
        {"dim", required_argument, NULL, 'd'},
        {"inputs", required_argument, NULL, 'i'},
        // {"outputs", required_argument, NULL, 'n'},
        {"max_num_iter", required_argument, NULL, 'm'},
        {"threshold", no_argument, NULL, 't'},
        {"seed", no_argument, NULL, 's'},
        {"centroids", no_argument, 0, 'c'},
        {"implementation", no_argument, NULL, 'a'}
    };

    int ind, cmd;
    while ((cmd = getopt_long(argc, argv, "k:d:i:m:t:s:ca:", l_opts, &ind)) != -1)
    {
        switch (cmd)
        {
        case 0:
            break;
        case 'k':
            opts->num_cluster = atoi((char *)optarg);
            break;
        case 'd':
            opts->dims = atoi((char *)optarg);
            break;
        case 'i':
            opts->inputs = (char *)optarg;
            break;
        case 'm':
            opts->max_num_iter = atoi((char *)optarg);
            break;
        case 't':
            opts->threshold = atof((char *)optarg);
            break;
        case 'c':
            opts->c = true;
            break;
        case 's':
            opts->seed = atoi((char *)optarg);
            break;
        case 'a':
            opts->implementation = atoi((char *)optarg);
            break;
        case ':':
            std::cerr << argv[0] << ": option -" << (char)optopt << "requires an argument." << std::endl;
            exit(1);
        }
    }
}
