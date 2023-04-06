#ifndef PRINTER_FILE_CONFIGURATOR
#define PRINTER_FILE_CONFIGURATOR

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <glob.h>
#include <vector>

using namespace std;

class PrinterfileConvertor {
    string printerFileLocation{"/home/einar/dev/google/as400code/AS400/PRTF/"};
    string outputFileLocation{"/tmp/java/"};
    string pageLineCount = "64";
    string pageCharCount = "134";
    ofstream ofs;
    int RecordFormatCount = 0;
    string Pos = "";
    string Const = "";

    string positioning(string str);

    void constContinuation(string line);

    void constant(string line);

    void keywordNoParam(string line);

    void keyword(string line);

    void position(string line);

    string fieldType(string type, string length, string decimalPosition);

    void field(string line);

    void pageSize(string line);

    void recordFormat(string line);

public:
    PrinterfileConvertor(string printerfileLocationPath, string outputLocationPath);
    ~PrinterfileConvertor();
    void convert(string fileName);
    void convert();
};
#endif

static inline string trim(string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

static void capitalize(string &str) {
    if (str.length() > 0) {
        str[0] = toupper(str[0]);
        for (int i = 1; i < str.length(); i++) {
            str[i] = tolower(str[i]);
        }
    }
}

PrinterfileConvertor::PrinterfileConvertor(string printerfileLocationPath, string outputLocationPath) {
    printerFileLocation = printerfileLocationPath;
    outputFileLocation = outputLocationPath;
}

PrinterfileConvertor::~PrinterfileConvertor() {}

void PrinterfileConvertor::constContinuation(string line) {
    regex regexp("^.{5}A\\s{38}\\'?(.*?)\\-\\s*$");
    smatch mtch;
    if (regex_search(line, mtch, regexp)) {
        if (1 < mtch.size()) {
            Const = regex_replace(string(mtch[1]), regex("[\"]"), "\\\"");
        }
    }
}

void PrinterfileConvertor::constant(string line) {
    regex regexp("^.{5}A\\s{38}\\'?(.*?)\\'\\s*$");
    smatch mtch;
    if (regex_search(line, mtch, regexp)) {
        if (1 < mtch.size()) {
            string p = positioning(Pos);
            string rest = regex_replace(string(mtch[1]), regex("[\"]"), "\\\"");
            ofs << "\t\t\t\t.appendConst(" << p << ", \"" + Const << rest << "\")" << endl;
            Const = "";
        }
    }
}

void PrinterfileConvertor::keywordNoParam(string line) {
    regex regexp("^.{5}A  ([\\d ]{2}) {34}(\\w+)\\s*$");
    smatch mtch;
    if (regex_search(line, mtch, regexp)) {
        if (2 < mtch.size()) {
            string indicator = trim(mtch[1]);
            string kw = trim(mtch[2]);
            string pos = regex_match(kw, regex("^DATE|TIME$")) ? positioning(Pos) : "";
            pos += ((pos.length() > 0 && indicator.length() > 0) ? " ," : "");
            string ind = (indicator.length() > 0) ? (" Indicator.value(" + indicator + ")")  : "";
            ofs << "\t\t\t\t.apply" << kw << "(" << pos << ind << ")" << endl;
        }
    }
}

void PrinterfileConvertor::keyword(string line) {
    regex regexp("^.{5}A  ([\\d ]{2}) {34}(\\w+)\\s*[(](.+?)[)]?\\s*[^'-]$");
    smatch mtch;
    if (regex_search(line, mtch, regexp)) {
        if (3 < mtch.size()) {
            string indicator = trim(mtch[1]);
            string kw = trim(mtch[2]);
            string param = trim(mtch[3]);
            if ((regex_match(param, regex("^\\*SYS \\*YY$")) || regex_match(param, regex("^\\*SYS$"))) && kw == "DATE") {
                param = "WhatDate.SYS";
            } else if (regex_match(param, regex("^\\*JOB$")) && kw == "DATE") {
                param = "WhatDate.JOB";
            } else if (regex_match(param, regex("^[1234ABCDJLMNOPQ].*")) && kw == "EDTCDE") {
                param = "EditCode.NumFormat_" + trim(regex_replace(param, regex("[^1234ABCDJLMNOPQ]"), ""));
            } else if (regex_match(param, regex("^[WYZ]$")) && kw == "EDTCDE") {
                param = "EditCode.DateFormat_" + param;
            } else if (regex_match(param, regex("^\\'.+\\'$"))) {
                param = regex_replace(param, regex("[']"), "\"");
            } else if (regex_match(param, regex("^\\d+$"))) {
                param =  to_string(stoi(param));
            } else {
                param = "\"" + param + "\"";
            }
            string pos = regex_match(kw, regex("^DATE|TIME$")) ? (positioning(Pos) + ", ") : "";
            param += (indicator.length() > 0) ? " ," : "";
            string ind = (indicator.length() > 0) ? (" Indicator.value(" + indicator + ")") : "";
            ofs << "\t\t\t\t.apply" << kw << "(" << pos << param << ind << ")" << endl;
        }
    }
}

void PrinterfileConvertor::position(string line) {
    regex regexp("^.{5}A\\s+([+ ])(\\d+)\\s*$");
    smatch mtch;
    if (regex_search(line, mtch, regexp)) {
        if (2 < mtch.size()) {
            Pos = trim(mtch[1]) + to_string(stoi(trim(mtch[2])));
        }
    }
}

string PrinterfileConvertor::positioning(string p) {
    regex regexp("\\+\\d+");
    smatch mtch;
    if (regex_search(p, mtch, regexp)) {
        return "relativePosition(" + p + ")";
    }
    return p;
}

string PrinterfileConvertor::fieldType(string type, string length, string decimalPosition) {
    if (0 == decimalPosition.length()) {
        decimalPosition = "0";
    }
    if (type == "S") {
        return "FieldType.Zoned(" + length + ", " + decimalPosition + ")";
    } else if (type == "F") {
        return "FieldType.Float(" + length + ", " + decimalPosition + ")";
    } else if (type == "A") {
        return "FieldType.Alphanumeric(" + length + ")";
    }
    return "FieldType." + type + "()";
}

void PrinterfileConvertor::field(string line) {
    regex regexp("^.{5}A {12}([ \\w]{10})\\s([ \\d]{5})([ASFLTZ])([ \\d]{2})([ OP])([ \\d]{3})([ \\+\\d]{3}).*$");
    smatch mtch;
    if (regex_search(line, mtch, regexp)) {
        if (7 < mtch.size()) {
            string name = trim(mtch[1]);
            string length = trim(mtch[2]);
            string type = trim(mtch[3]);
            string decimalPosition = trim(mtch[4]);
            string usage = trim(mtch[5]);
            string line = trim(mtch[6]);
            string position = trim(mtch[7]);
            ofs << "\t\t\t\t.appendField(" << positioning(position) << ", \"" << name << "\", " <<
                fieldType(type, length, decimalPosition) << ")" << endl;
        }
    }
}

void PrinterfileConvertor::pageSize(string line) {
    regex regexp("^\\s{5}A[*][%]{2}C[P][\\s+]\\s{4}PAGESIZE[(][*]?(\\w+)[\\s*]+(\\w+)[\\s*]*(\\w+)?\\s*[)]");
    smatch mtch;
    if (regex_search(line, mtch, regexp)) {
        if (1 < mtch.size() && trim(mtch[1]) != "N") {
            pageLineCount = to_string(stoi(trim(mtch[1])));
        }
        if (2 < mtch.size() && trim(mtch[2]) != "N") {
            pageCharCount = to_string(stoi(trim(mtch[2])));
        }
    }
}

void PrinterfileConvertor::recordFormat(string line) {
    regex regexp("^.{5}A {10}R (\\w+)");
    smatch mtch;
    if (regex_search(line, mtch, regexp)) {
        if (1 < mtch.size()) {
            if (RecordFormatCount > 0) {
                ofs << "\t\t\t)" << endl;
            }
            ofs << "\t\t\t.addRecordFormat(RecordFormat.create(\"" << trim(mtch[1]) << "\")" << endl;
            RecordFormatCount++;
        }
    }
}

void PrinterfileConvertor::convert(string fileName) {
    string className{fileName};
    capitalize(className);
    ofs.open(outputFileLocation + className + ".java");
    if (ofs.is_open()) {
        cout << "converting file " << fileName << endl;
        ofs << "package nl.cornerstone.as400.prtf.reports;\n" << endl;
        ofs << "import static nl.cornerstone.as400.prtf.PrinterFile.relativePosition;" << endl;
        ofs << "import nl.cornerstone.as400.prtf.keywords.DATE.WhatDate;" << endl;
        ofs << "import nl.cornerstone.as400.prtf.keywords.EDTCDE.EditCode;" << endl;
        ofs << "import nl.cornerstone.as400.prtf.*;" << endl;
        ofs << "\n\npublic class " << className << " {" << endl;
        ofs << "\tpublic static PrinterFile create() {\n\t\treturn PrinterFile.create(\"" << fileName << "\")" << endl;
        ifstream ifs(printerFileLocation + fileName);
        if (ifs.is_open()) {
            string line;
            while (getline(ifs, line)) {
                recordFormat(line);
                field(line);
                keywordNoParam(line);
                keyword(line);
                constContinuation(line);
                constant(line);
                position(line);
                pageSize(line);
                //cout << line << endl;
            }
            ifs.close();
        } else {
            cerr << "Could not open input file " << printerFileLocation + fileName << endl;
        }
        ofs << "\t\t\t\t)\n\t\t\t.pageSize(" << pageLineCount << "," << pageCharCount << ");\n\t}\n}" << endl;
        ofs.close();
    } else {
        cerr << "Could not open output file " << outputFileLocation + className +".java" << endl;
    }
}

static inline string getFilename(string path) {
    regex regexp("^.+/(\\w+)$");
    smatch mtch;
    if (regex_search(path, mtch, regexp)) {
        if (1 < mtch.size()) {
            return trim(mtch[1]);
        }
    }
    return "";
}

static vector<string> getFilenamesInsideFolder(const string& pattern){
    glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> files;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        string filename = getFilename(string(glob_result.gl_pathv[i]));
        files.push_back(filename);
    }
    globfree(&glob_result);
    return files;
}

static void convertAllFiles(string printerFileLocation, string outputLocation) {
    for (auto filename : getFilenamesInsideFolder(printerFileLocation + "*")) {
        PrinterfileConvertor convertor(printerFileLocation, outputLocation);
        convertor.convert(filename);
    }
}

/**
 * Example run :
 * ./printerfile_converter ~/dev/google/as400code/AS400/PRTF/ ~/dev/google/converted-online-batch-transactions/Build/boavista-as400/printerfiles/src/main/java/nl/cornerstone/as400/prtf/reports/
 */
int main(int ac, char *av[]) {
    if (ac < 3){
        cerr << "Usage: \n" << av[0] << " printer-file-location-path output-location-path" << endl;
        return 1;
    }
    string printerFileLocation {av[1]};
    string outputLocation {av[2]};
    convertAllFiles(printerFileLocation + "/", outputLocation + "/");
    cout << "END RUN" << endl;
    return 0;
}
