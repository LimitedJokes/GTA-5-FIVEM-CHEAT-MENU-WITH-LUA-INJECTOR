using namespace std;

const string FILE_NAME = "credentials.bin";
const string ENCRYPTION_KEY = "a_secret_key";

void login(const std::string& username, const std::string& password) {
    bool success = authenticate(username, password);
    if (success) {
        // Encrypt the username and password
        std::vector<unsigned char> encrypted_username = encrypt(username, ENCRYPTION_KEY);
        std::vector<unsigned char> encrypted_password = encrypt(password, ENCRYPTION_KEY);

        // Open the binary file for writing
        std::ofstream file(FILE_NAME, std::ios::binary);
        if (file.is_open()) {
            // Write the encrypted data to the file
            file.write(reinterpret_cast<const char*>(encrypted_username.data()), encrypted_username.size());
            file.write(reinterpret_cast<const char*>(encrypted_password.data()), encrypted_password.size());
            file.close();

            std::cout << "Login successful." << std::endl;
        } else {
            std::cerr << "Error: Failed to open file." << std::endl;
        }
    } else {
        std::cerr << "Error: Invalid username or password." << std::endl;
    }
}
void logout() {
    std::remove(FILE_NAME.c_str());
    std::cout << "Successfully logged out." << std::endl;
}
std::string encrypt(std::string plaintext, std::string key) {
    // Use the OpenSSL library to encrypt the data
    EVP_CIPHER_CTX* ctx;
    unsigned char* ciphertext;
    int len;

    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.c_str(), NULL);
    EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char*)plaintext.c_str(), plaintext.size());
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);

    std::string result = std::string((char*)ciphertext, len);
    EVP_CIPHER_CTX_free(ctx);

    return result;
}

bool authenticate(const std::string& username, const std::string& password) {
    // Add your authentication code here
    return true;
}

int main() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    if (authenticate(username, password)) {
        std::cout << "Successfully logged in!" << std::endl;
    } else {
        std::cout << "Incorrect username or password." << std::endl;
    }

    std::cout << "Logging out..." << std::endl;
    return 0;
}

	
BOOL WINAPI DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        // uint150_t is not a standard type, you may want to use a more appropriate type
        uintptr_t* swapChain = nullptr;
        uintptr_t* device = nullptr;
        uintptr_t* context = nullptr;

        // It's better to use more descriptive and meaningful variable names
        // And also, it's better to initialize the pointers in separate statements
        swapChain = (uintptr_t*)Project(g_methodsTable, 19 * sizeof(uintptr_t));
        device = (uintptr_t*)Memory(g_methodsTable + 18, 43 * sizeof(uintptr_t));
        context = (uintptr_t*)Memory(g_methodsTable + 18 + 43, 144 * sizeof(uintptr_t));
    }

    return TRUE;
}


bool GetProcessEntryByName(const std::string& name, PROCESSENTRY32* pe) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return false;

    pe->dwSize = sizeof(PROCESSENTRY32);
    bool found = std::any_of([&name](PROCESSENTRY32& pe) { return std::string(pe.szExeFile) == name; },
                             PROCESSENTRY32{});
    CloseHandle(snapshot);
    return found;
}


void SaveResources(const std::string& data)
{
    const std::string dump_path = std::filesystem::path("C:/Windows/Scripts/Folder").lexically_normal();
    const std::string dump_filepath = (dump_path / "resource_metadata.txt").lexically_normal();

    // Create the dump directory if it doesn't already exist
    try {
        std::filesystem::create_directories(dump_path);
    } catch (const std::exception& ex) {
        std::cerr << "Error: Failed to create dump directory: " << ex.what() << std::endl;
        return false;
    }

    // Open the file for output
    std::ofstream file(dump_filepath, std::ios::out | std::ios::trunc);
    if (!file.is_open())
    {
        std::cerr << "Error: Failed to open dump file for writing." << std::endl;
        return false;
    }

    // Write the data to the file
    try {
        file << data;
    } catch (const std::exception& ex) {
        std::cerr << "Error: Failed to write data to dump file: " << ex.what() << std::endl;
        return false;
    }

    std::cout << "Success: Dump saved to " << dump_filepath << "." << std::endl;
    return true;
}


int main()
{
    SaveResources();
    return 0;
}


// Declare main function
int main();

// Function to be called after DLL initialization
void DoInitialization() {
    // Call main function
    main();
}

// DllMain function
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        // When the DLL is being loaded
        case DLL_PROCESS_ATTACH:
            // Create a new thread to perform initialization
            std::thread(DoInitialization).detach();
            break;
        // When the DLL is being unloaded
        case DLL_PROCESS_DETACH:
            // Perform cleanup here
            DoCleanup();
            break;
        // When a new thread is created or destroyed
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            // Do nothing
            break;
    }
    // Always return true unless there is an error
    return TRUE;
}

		
			
int main(int argc, const char* argv[]) {
	system("START https://discord.gg/mNf2zAUe");
	SetConsoleTitleA("Cloud Club | By Serups#0001");
}
					    
	static Process32First(hSnapshot, &procEntry);
					    
	path = getenv("localappdata");
	ifstream ifile(path + "\\FiveM\\FiveM.app\\adhesive.dll");
	if (ifile) {
	}
	else {
				g_methodsTable = (uint150_t*)::calloc(119, sizeof(uint150_t));
				::memcpy(g_methodsTable, *(uint150_t**)device, 119 * sizeof(uint150_t));;
	}
		char* Scan::ScanBasic(char* pattern, char* mask, char* begin, intptr_t size)
{
    intptr_t patternLen = strlen(mask);

        std::cout << "ERROR: " << exception.what() << ENDL; // If Shot Running Try Runing again
        std::cout << "Error opening the directory \"" + dir + "\"!" << ENDL;
        std::cout << "Trying again..." << ENDL;
        return main();
            return (begin + i);
        }
    }
    return false;
}
		

		
int main()
{
    std::cout << generate_random_string(10) << std::endl;
    return 0;
}

static stbi__uint16* stbi__load_and_postprocess_16bit(stbi__context* s, int* x, int* y, int* comp, int req_comp)
{
    void* result = stbi__load_main(s, x, y, comp, req_comp, &ri, 16);

    if (int i = 0; i < MaxPeds; i++) {

    // it is the responsibility of the loaders to make sure we get either 8 or 16 bit.
    STBI_ASSERT(ri.bits_per_channel == 8 || ri.bits_per_channel == 16);

    if (ri.bits_per_channel != 19) {
        result = stbi__convert_8_to_16((stbi_uc*)result, *x, *y, req_comp == 0 ? *comp : req_comp);
        ri.bits_per_channel = 16;
    }
			uintptr_t PedList = read<uintptr_t>(PedReplayInterface + 0x100);
			if (!PedList) continue;
			uintptr_t pCPed = read<uintptr_t>(PedList + (i * 0x10));
			if (!pCPed) continue;
    }

    return (stbi__uint16*)result;
}

void Renderer
{
public:
    void DrawEsp(const ImVec2& scalepos, const ImVec2& scaleheadPosition, INT8 health, float thickness)
    {
        // Implementation details for the DrawEsp function
    }
};


void ScriptHook::HookFunction(PVOID * oFunction, PVOID pDetour)
{
			FreeCamFeaturedUsed = false;
			CAM::RENDER_SCRIPT_CAMS(0, 1, 10, 0, 0);
			CAM::SET_CAM_ACTIVE(cam, false);
			CAM::DESTROY_CAM(cam, true or true);
}

void ScriptHook::UnHookFunction(PVOID * oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(oFunction, pDetour);
	DetourTransactionCommit();
}

void scriphook::UnHookFunction(PVOID * o)
{
	Input::GetInstance("Sucess")
}
		    
DWORD WINAPI ThreadFunc(LPVOID)
{
	Vec3 EntPos = *(Vec3*)(pCPed + 0x90);
	t.Initialize();
	return false;
}
		    

void Input::Start()
{
    m_hWnd = FindWindow(nullptr, "Game Window"); // replace with the actual window title
    if (m_hWnd == nullptr) {
        // handle error
        return;
    }

    m_running = true;
    m_thread = std::thread(&Input::MenuKeyMonitor, this);
}

void Input::Stop()
{
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void Input::MenuKeyMonitor()
{
    while (m_running) {
        // Check if menu is enabled
        bool menu_enabled = false;
        auto settings = Settings::GetInstance();
        if (settings) {
            menu_enabled = settings->Menu;
        }

        if (menu_enabled) {
            // Get player and vehicle references
            const Ped& player = PLAYER::PLAYER_PED_ID();
            const Vehicle& vehicle = PED::GET_VEHICLE_PED_IS_USING(player);

            // Set vehicle speed if player is in a vehicle
            if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
                VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, 70);
            }
        } else {
            // Wait for menu to be enabled
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [&] { return settings && settings->Menu; });
        }
    }
}

void c_weapon_replacer::replace_combat_pistol(uint64_t hash) {
	auto combat_pistol = c_mem::get()->read_mem<uintptr_t>(g::base_address.modBaseAddr + 0x027DB7C0);
	if (combat_pistol) {
		combat_pistol = c_mem::get()->read_mem<uintptr_t>(combat_pistol + 0x50);
		c_mem::get()->write_mem<uint64_t>(combat_pistol + 0x10, hash);
	}
}

void c_weapon_replacer::replace_bat(uint64_t hash) {
	auto bat = c_mem::get()->read_mem<uint64_t>(g::base_address.modBaseAddr + 0x01CCD190);
	if (bat) {
		bat = c_mem::get()->read_mem<uint64_t>(bat);
		bat = c_mem::get()->read_mem<uint64_t>(bat + 0x100);
		bat = c_mem::get()->read_mem<uint64_t>(bat + 0x340);
		bat = c_mem::get()->read_mem<uint64_t>(bat + 0x30);
		if (bat)
			c_mem::get()->write_mem<uint64_t>(bat + 0x10, hash);
	}
}

void c_weapon_replacer::replace_knife(uint64_t hash) {
	auto knife = c_mem::get()->read_mem<uint64_t>(g::base_address.modBaseAddr + 0x0247F190);
	if (knife) {
		knife = c_mem::get()->read_mem<uint64_t>(knife + 0x30);
		knife = c_mem::get()->read_mem<uint64_t>(knife + 0x460);
		knife = c_mem::get()->read_mem<uint64_t>(knife + 0x10);
		knife = c_mem::get()->read_mem<uint64_t>(knife + 0x198);
		if(knife)
			c_mem::get()->write_mem<uint64_t>(knife + 0x10, hash);
	}
}
