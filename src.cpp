#include <iostream>
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

bool command_exists()
{
    bool exists = true;
    // pdfcrop
    if (system("which pdfcrop > /dev/null 2>&1"))
    {
        std::cerr << "[Error] Command 'pdfcrop' not found, it can be installed with:"
        << std::endl;
        std::cerr << "pdf2eps -i" << std::endl;

        exists = false;
    }

    // pdftops
    if (system("which pdftops > /dev/null 2>&1"))
    {
        std::cerr << "[Error] Command 'pdftops' not found, it can be installed with:"
        << std::endl;
        std::cerr << "pdf2eps -i" << std::endl;

        exists = false;
    }

    // pdftk
    if (system("which pdftk > /dev/null 2>&1"))
    {
        std::cerr << "[Error] Command 'pdftk' not found, it can be installed with:"
        << std::endl;
        std::cerr << "pdf2eps -i" << std::endl;

        exists = false;
    }

    return exists;
}

void install_commands()
{
    system("sudo add-apt-repository ppa:malteworld/ppa");
    system("sudo apt update");
    system("sudo apt install -y texlive-extra-utils poppler-utils pdftk");
}

int process_file(
    fs::path src,
    fs::path dst,
    std::string pdfcrop_args,
    std::string pdftops_args,
    bool eps
)
{
    fs::path dir_name = src.parent_path();
    fs::path file_name = src.filename();
    fs::path out_name = dst / file_name;

    std::string command = (boost::format("pdfcrop %1% %2% tmp.pdf")
        % pdfcrop_args % src.string() ).str();
    system(command.c_str());

    if(eps)
    {
        command = (boost::format("pdftops %1% tmp.pdf %2%")
            % pdftops_args % out_name.replace_extension(".eps") ).str();
        system(command.c_str());
    } else
    {
        fs::copy_file(fs::path("./tmp.pdf"), out_name, fs::copy_option::overwrite_if_exists);
    }
    fs::remove(fs::path("./tmp.pdf"));

    return 0;
}

int process_split(
    fs::path src,
    fs::path dst,
    std::string pdfcrop_args,
    std::string pdftops_args,
    bool eps
)
{
    fs::path dir_name = src.parent_path();
    fs::path file_name = src.filename();
    fs::path base_name = src.stem();

    boost::system::error_code error;
    bool result = fs::exists("./tmp", error);
    if(!result || error){
        result = fs::create_directories("./tmp", error);
        if(!result || error)
        {
            std::cerr << "Failued to create temporary directory ./tmp" << std::endl;
            return -1;
        }
    }
    
    result = fs::exists((dst/base_name), error);
    if(!result || error){
        result = fs::create_directories((dst/base_name), error);
        if(!result || error)
        {
            std::cerr << "Failued to create destination directory" << dst/base_name << std::endl;
            return -1;
        }
    }

    std::string command = (boost::format("pdftk %1% burst output")
        % src.string() ).str() + " ./tmp/file_\%04d.pdf";
    system(command.c_str());

    int ret;
    fs::directory_iterator end_itr;
    for(fs::directory_iterator itr("./tmp"); itr!= end_itr; itr++)
    {
        if(itr->path().extension() == ".pdf")
            ret = process_file(itr->path(), (dst/base_name), pdfcrop_args, pdftops_args, eps);
    }

    result = fs::remove_all("./tmp", error);

    return ret;

}

int main(int argc, char** argv)
{
    // PROGRAM VERSION
    const std::string VERSION = "pdf2eps 3.0.0";

    // 内部変数
    fs::path src, dst;
    bool batch, split, eps;
    std::string pdfcrop_args = "";
    std::string pdftops_args = "";
    std::string margins;

    // Program options
    // 名前付き引数
    po::options_description opt("Options");
    opt.add_options()
        ("help, h", "(--h) print this")
        ("version, v", "(--v) print this script's version")
        ("install, i", "(--i) install pdfcrop, pdftops, and pdftk")
        ("batch, b", "(--b) batch process all PDF files in the folder")
        ("divide, d", "(--d) divide PDF file in each page")
        ("eps, e", "(--e) output as EPS file")
        ("margins", po::value<std::vector<int>>()->multitoken(),
        "[PDFcrop option] <left> <top> <right> <bottom>,\ndefault=10 10 10 10")
        ("output, o", po::value<std::string>(),"(--o) destination directory\ndefault='./'")
        ("src", po::value<std::string>(),"(positional) source file/folder")
        ;
    
    // 位置引数
    po::positional_options_description pos;
    pos.add("src", 1);
    
    po::command_line_parser parser{argc, argv};
    parser.options(opt).positional(pos);

    po::variables_map vm{};
    try{
        po::store(parser.run(), vm);
        po::notify(vm);
    } catch (po::error& e){
        std::cerr << "[Error] " << e.what() << std::endl;
        std::cerr << opt << std::endl;
        return 1;
    }

    if(vm.count("help"))
    {
        std::cout << opt << std::endl;
        return 0;
    }   
    
    if(vm.count("version"))
    {
        std::cout << VERSION << std::endl;
        return 0;
    }
    
    if(vm.count("install"))
    {
        std::cout << "Installing texlive-extra-utils, poppler-utils, and pdftk"
        << std::endl;
        install_commands();
        return 0;
    }

    if(vm.count("batch"))
    {
        batch = true;
    } else
    {
        batch = false;
    }

    if(vm.count("divide"))
    {
        split = true;
    } else
    {
        split = false;
    }

    if(vm.count("eps"))
    {
        eps = true;
    } else
    {
        eps = false;
    }

    if(vm.count("margins"))
    {
        std::vector<int> tmp = vm["margins"].as<std::vector<int>>();
        if(tmp.size()==1)
            margins = (boost::format(" --margins %1% ") %tmp[0]).str();
        if(tmp.size()==4)
            margins = (boost::format(" --margins '%1% %2% %3% %4%' ")
             % tmp[0] % tmp[1] % tmp[2] % tmp[3]).str();
    } else
    {
        margins = " --margins 10 ";
    }
    pdfcrop_args += margins;

    if(vm.count("output"))
    {
        dst = fs::path{vm["output"].as<std::string>()};

        // 出力フォルダが存在しなければ作る
        boost::system::error_code error;
        bool result = fs::exists(dst, error);
        if(!result || error)
        {
            result = fs::create_directories(dst, error);
            if(!result || error)
            {
                std::cerr << "Failued to create output directory" << std::endl;
                return -1;
            }
        }
    }

    if(vm.count("src"))
    {
        src = fs::path{vm["src"].as<std::string>()};

        // 存在チェック
        boost::system::error_code error;
        bool result = fs::exists(src, error);
        if(!result || error)
        {
            std::cerr << "[Error]" << src << "is not found" << std::endl;
            return -1;
        }
        result = fs::is_directory(src, error);
        if(!result || error)
        {   // ファイル
            if(batch)
            {
                std::cerr << "[Error] batch mode can't apply to file" << std::endl;
                std::cerr << "remove --b option" << std::endl;
                return -1;
            }
        } else
        {
            if(!batch)
            {
                std::cerr << "[Error] file mode can't apply to folder" << std::endl;
                std::cout << "use --b option for batch mode" << std::endl;
                return -1;
            }
        }
    } else
    {
        std::cout << opt << std::endl;
        return 0;
    }

    // 環境チェック
    int ret = system(nullptr);
    if (ret == 0)
    {
        std::cout << "Shell is not available" << std::endl;
        return -1;
    }
    
    if(!command_exists()) return -1;

    std::cout << "##### Options #############" << std::endl;
    std::cout << "Batch process            : " << std::boolalpha << batch << std::endl;
    std::cout << "Split PDF with each pages: " << std::boolalpha << split << std::endl;
    std::cout << "Output as eps            : " << std::boolalpha << eps << std::endl;
    std::cout << "pdfcrop arguments        : " << pdfcrop_args << std::endl;
    std::cout << "pdftops arguments        : " << pdftops_args << std::endl;
    std::cout << "Source file/folder       : " << src << std::endl;
    std::cout << "Destination file/folder  : " << dst << std::endl;
    std::cout << "###########################" << std::endl;

    // メイン処理
    if(batch)
    {
        boost::filesystem::directory_iterator end_itr;
        for(fs::directory_iterator itr(src); itr!= end_itr; itr++)
        {
            if(split)
            {
                if(itr->path().extension() == ".pdf")
                    ret = process_split(itr->path(), dst, pdfcrop_args, pdftops_args, eps);
            } else
            {
                if(itr->path().extension() == ".pdf")
                    ret = process_file(itr->path(), dst, pdfcrop_args, pdftops_args, eps);
            }
        }
    } else
    {
        if(split)
        {
            ret = process_split(src, dst, pdfcrop_args, pdftops_args, eps);
        } else
        {
            ret = process_file(src, dst, pdfcrop_args, pdftops_args, eps);
        }
    }

    return ret;
}