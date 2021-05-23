#include "test/crailnative_test.hh"

#include <memory>
#include <fstream>

void show_usage(void) {
    printf("usage: \
    crailnative_test [ip] [port] [key] \n \
    [key] without '/' \n \
     \
    ");
}

int main(int argc, char* argv[]) {
    std::string addr;
    int port;
    bool run_once = false;

    if (argc >= 3) {
        addr = argv[1];
        port = atoi(argv[2]);

        printf("read arg from argv: \n \
         [IP]: %s -> [PORT]: %d\n", addr.c_str(), port);
    } else {
        std::cout << "enter addr:";
        std::cin >> addr;

        std::cout << "enter port:";
        std::cin >> port;
    }

    std::shared_ptr<CrailStore> crailStore;
    crailStore.reset(new CrailStore(addr, port));

    printf("<-[INFO] begin connect to crail\n");
    crailStore->Initialize();
    printf("<-[INFO] connect to crail end\n");

    while(1) {
        std::string key = "";
        std::string file_name = "";
        string str_data = "";
        
        if (argc >= 4) {
            key = argv[3];
            run_once = true;

            printf("read [KEY] from argv. will exit after finish write!\n");
        } else {
            std::cout << "enter key to download:";
            std::cin >> key;
        }
        
        file_name = key;
        key = "/" + key;

        printf("[INFO] key is [%s]\n", key.c_str());

        CrailFile file = crailStore->Lookup<CrailFile>(const_cast<std::string&>(key)).get();
        if (!file.valid()) {
            fprintf(stderr, "[ERROR] lookup node failed\n");
            return -1;
        }

        printf("<-[NOTICE] [download_files] filename: [%s], object_key: [%s]\n", file_name.c_str(), key.c_str());

        unique_ptr<CrailInputstream> inputstream = file.inputstream();
        shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(kBufferSize);

        while (inputstream->Read(buf).get() > 0) {
            buf->Flip();
            str_data.append(reinterpret_cast<const char*>(buf->get_bytes()), buf->remaining());
            buf->Clear();
        } 

        inputstream->Close();

        printf("[INFO] begin write file [%s]\n", file_name.c_str());
        std::ofstream ofs(file_name);
        ofs << str_data;
        ofs.close();
        printf("[INFO] end write file [%s]\n", file_name.c_str());

        printf("[INFO] begin calc file hash\n");
        std::string command = "md5sum ";
        command = command + file_name;
        ::system(command.c_str());
        printf("[INFO] end calc file hash\n");

        if (run_once == true) {
            break;
        }
    }

    return 0;
}