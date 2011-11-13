#ifndef _TEXTMODEL_H_
#define _TEXTMODEL_H_

#include <oberon.h>
#include <typeregister.h>
#include <store.h>

namespace odc {

	class TextModel : public ContainerModel {
		private:
		static const TypeProxy<TextModel, ContainerModel> PROXY;

		public:
		static const std::string TYPENAME;
		virtual const std::string &getTypeName() const;

		TextModel(INTEGER id);
		/**
		 * Just calls super and reads the version and checks that its in the allowed range.
		 */
		virtual void internalize(Reader &reader);
	};

	/**
	 * A TextPiece is just a component of an StdTextModel.
	 * It has a certain length in bytes, which is known construction time (due
	 * to meta-data in the StdTextModel header) and contents which are read
	 * later.
	 */
	class TextPiece {
		public:
		/**
		 * The number of bytes that will be read.
		 */
		const size_t d_len;
		TextPiece(size_t len);
		virtual void read(Reader &reader) = 0;
		virtual std::string toString() const = 0;
		virtual void accept(Visitor &visitor) const = 0;
		/**
		 * Size in bytes, excluding the null-character that terminates the string (i.e. the size that is read from file).
		 */
		unsigned size() const;
	};

	/**
	 * TextPiece consisting of 16-bit unicode characters.
	 * Not sure if the encoding is UCS-2 or UTF-16.
	 */
	class LongPiece : public TextPiece {
		private:
		CHAR *d_buf;
		public:
		LongPiece(size_t len);
		~LongPiece();
		virtual void read(Reader &reader);
		virtual std::string toString() const;
		virtual void accept(Visitor &visitor) const;

		/**
		 * Get the buffer contents as 16-bit (UCS-2 or UTF-16 I don't know) unicode.
		 */
		CHAR *getBuffer() const;
	};

	/**
	 * TextPiece consisting of 8-bit characters in the Latin-1 extension of ASCII.
	 */
	class ShortPiece : public TextPiece {
		private:
		SHORTCHAR *d_buf;
		public:
		ShortPiece(size_t len);
		~ShortPiece();
		virtual void read(Reader &reader);
		virtual std::string toString() const;
		virtual void accept(Visitor &visitor) const;

		/**
		 * Get the buffer contents as 8-bit (Latin-1) characters.
		 */
		SHORTCHAR *getBuffer() const;
	};

	/**
	 * TextPiece that embeds a View.
	 */
	class ViewPiece : public TextPiece {
		Store *d_view;
		public:
		ViewPiece(Store *view);
		~ViewPiece();
		virtual void read(Reader &reader);
		virtual std::string toString() const;
		virtual void accept(Visitor &visitor) const;
	};

	/**
	 * Default implementation of a TextModel.
	 * Essentially it is a series of TextPieces.
	 */
	class StdTextModel : public TextModel {
		private:
		static const TypeProxy<StdTextModel, TextModel> PROXY;
		std::vector<TextPiece *> d_pieces;

		public:
		static const std::string TYPENAME;
		virtual const std::string &getTypeName() const;

		StdTextModel(INTEGER id);
		/**
		 * Calls super and reads the version and checks that its in the allowed range.
		 * Then the text model meta-data is read, including information on all the pieces.
		 * After reading the meta-data, the pieces themselves are read.
		 */
		virtual void internalize(Reader &reader);

		virtual std::string toString();
		virtual void accept(Visitor &visitor) const;
	};

} // namespace odc

#endif // _TEXTMODEL_H_
