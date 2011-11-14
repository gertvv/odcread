#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include <oberon.h>
#include <reader.h>
#include <store.h>
#include <textmodel.h>
#include <visitor.h>

// Character encoding conversions
#include <locale.h>
#include <iconv.h>
#include <errno.h>
#include <string.h>

namespace odc {
	class Context {
		public:
		virtual void addPiece(std::string &piece) = 0;
		virtual std::string getPlainText() const = 0;
	};
	class PartContext : public Context {
		private:
		std::string d_text;
		public:
		virtual void addPiece(std::string &piece) {
			d_text += piece;
		}
		virtual std::string getPlainText() const {
			return d_text;
		}
	};
	class FoldContext : public Context {
		private:
		bool d_collapsed;
		bool d_haveFirst; // flag that first part has been set
		std::string d_firstPart;
		std::string d_remainder;
		public:
		FoldContext(bool collapsed) : d_collapsed(collapsed), d_haveFirst(false) {}
		virtual void addPiece(std::string &piece) {
			if (!d_haveFirst) {
				d_haveFirst = true;
				d_firstPart = piece;
			} else {
				d_remainder += piece;
			}
		}
		virtual std::string getPlainText() const {
			if (d_collapsed) {
				return std::string("##=>") + d_remainder + "\n" + d_firstPart +"##<=";
			} else {
				return std::string("##=>") + d_firstPart + "\n" + d_remainder +"##<=";
			}
		}
	};

	class MyVisitor : public Visitor {
		private:
		std::stack<Context*> d_context;

		void terminateContext() {
			Context *c = d_context.top();
			d_context.pop();
			if (d_context.empty()) {
				std::cout << c->getPlainText() << std::endl;
			} else {
				std::string text = c->getPlainText();
				d_context.top()->addPiece(text);
			}
			delete c;
		}
		
		public:
		virtual void partStart() {
			d_context.push(new PartContext());
		}
		virtual void partEnd() {
			terminateContext();
		}
		virtual void foldLeft(bool collapsed) {
			d_context.push(new FoldContext(collapsed));
		}
		virtual void foldRight() {
			terminateContext();
		}
		char *getCharSet() {
			return "UTF-8"; // FIXME setlocale(LC_CTYPE, 0) + processing
		}
		virtual void textShortPiece(const ShortPiece *piece) {
			iconv_t conv = iconv_open("UTF-8", "ISO-8859-1");
			if (conv == (iconv_t)-1) {
				std::string str("iconv initialization error: ");
				str += strerror(errno);
				throw str.c_str();
			}
			size_t bytesIn = piece->size() + 1;
			SHORTCHAR *in = piece->getBuffer();
			size_t bytesOut = bytesIn; // FIXME probably not safe.
			char *out = new char[bytesIn];
			char *outPtr = out;
			size_t rval = iconv(conv, &in, &bytesIn, &outPtr, &bytesOut);
			if (rval == (size_t)-1) {
				std::string str("iconv error: ");
				str += strerror(errno);
				throw str.c_str();
			}
			iconv_close(conv);
			std::string str(out);
			for (std::string::iterator it = str.begin(); it < str.end(); ++it) {
				if (*it == '\r') *it = '\n';
			}
			d_context.top()->addPiece(str);
		}
		virtual void textLongPiece(const LongPiece *piece) {
		/*	
			char *out = (char*)piece->getBuffer();
			std::string str(out);
			d_context.top()->addPiece(str);
		*/
			//d_convLong = iconv_open(setlocale(LC_CTYPE, 0), "UCS-2");
			iconv_t conv = iconv_open("UTF-8", "UCS-2");
			if (conv == (iconv_t)-1) {
				std::string str("iconv initialization error: ");
				str += strerror(errno);
				throw str.c_str();
			}
			size_t bytesIn = piece->size() + 2;
			char *in = (char*)piece->getBuffer();
			size_t bytesOut = bytesIn; // FIXME probably not safe.
			char *out = new char[bytesIn];
			char *outPtr = out;
			size_t rval = iconv(conv, &in, &bytesIn, &outPtr, &bytesOut);
			if (rval == (size_t)-1) {
				std::string str("iconv error: ");
				str += strerror(errno);
				throw str.c_str();
			}
			iconv_close(conv);
			std::string str(out);
			for (std::string::iterator it = str.begin(); it < str.end(); ++it) {
				if (*it == '\r') *it = '\n';
			}
			d_context.top()->addPiece(str);
		}
	};

	Store* importDocument(std::istream &is) {
		const INTEGER docTag = 0x6F4F4443;
		const INTEGER docVersion = 0;
		Reader r(is);
		INTEGER tag = r.readInt();
		if (tag == docTag) {
			INTEGER version = r.readInt();
			if (version != docVersion) {
				throw 100;
			}
			Store *s = r.readStore();
			return s;
		}
		return 0;
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		return 1;
	}

	// Set the locale according to the terminal's environment
	setlocale(LC_ALL, "");

	std::ifstream in(argv[1], std::ios::in | std::ios::binary);

	odc::Store* s;
	try {
		s = odc::importDocument(in);
	} catch (int trap) {
		std::cerr << "Exception in parsing file: BlackBox trap no. " << trap << std::endl;
		return 2;
	} catch (const char * exception) {
		std::cerr << "Exception in parsing file: " << exception << std::endl;
		return 2;
	}
//	std::cout << s->toPlainText() << std::endl;
//	std::cout << std::endl << std::endl;

	try {
		odc::MyVisitor visitor;
		s->accept(visitor);
	} catch (const char * exception) {
		std::cerr << "Exception in processing document: " << exception << std::endl;
		return 3;
	}
//	std::cout << s->toString() << std::endl;
//	std::cout << in.tellg() << " " << in.eof() << std::endl;

//	odc::TypePath path;
//	odc::ContainerModel(0).getTypePath(&path);
//	std::cout << path.toString() << std::endl;
	return 0;
}
