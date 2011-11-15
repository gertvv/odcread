#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "oberon.h"
#include "reader/reader.h"
#include "store/store.h"
#include "textmodel/textmodel.h"
#include "visitor/visitor.h"

// Character encoding conversions
#include <langinfo.h> // determine the current charset
#include <locale.h> // locale support
#include <iconv.h> // charset conversions
#include <errno.h> // error codes
#include <string.h> // string descriptions of error codes

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
			return nl_langinfo(CODESET);
		}
		virtual void textShortPiece(const ShortPiece *piece) {
			std::string str = convert((char *)piece->getBuffer(), piece->size() + 1, (char *)"ISO-8859-1", 1);
			d_context.top()->addPiece(str);
		}
		virtual void textLongPiece(const LongPiece *piece) {
			std::string str = convert((char *)piece->getBuffer(), piece->size() + 2, (char *)"UCS-2", 2);
			d_context.top()->addPiece(str);
		}

		/**
		 * Convert an input character buffer in the given encoding to the
		 * locale's encoding.
		 */
		std::string convert(char *in, size_t bytesIn, char *encodingIn, size_t inBytesPerChar) {
			// Convert from the input encoding to the locale's encoding
			iconv_t conv = iconv_open(getCharSet(), encodingIn);

			// Handle errors by throwing a readable message
			if (conv == (iconv_t)-1) {
				std::string str("iconv initialization error: ");
				str += strerror(errno);
				throw str.c_str();
			}

			// Assume at most 4 bytes per character are needed
			size_t bytesOut = 4 * bytesIn / inBytesPerChar;

			// Allocate the output buffer
			char *out = new char[bytesOut];
			char *outPtr = out;

			// Perform conversion
			size_t rval = iconv(conv, &in, &bytesIn, &outPtr, &bytesOut);
			if (rval == (size_t)-1) {
				std::string str("iconv error: ");
				str += strerror(errno);
				throw str.c_str();
			}

			// Free the iconv state
			iconv_close(conv);

			// Copy result into a std::string
			std::string str(out);
			delete out;

			// Convert newlines
			for (std::string::iterator it = str.begin(); it < str.end(); ++it) {
				if (*it == '\r') *it = '\n';
			}

			return str;
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
