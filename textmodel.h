#ifndef _TEXTMODEL_H_
#define _TEXTMODEL_H_

#include <oberon.h>
#include <typeregister.h>
#include <store.h>

namespace odc {

	class TextModel : public ContainerModel {
		private:
		static const std::string TYPENAME;
		static const TypeProxy<TextModel> PROXY;

		public:
		TextModel(INTEGER id);
		static const std::string &getType();
		static const std::string *getSuper();
		virtual const std::string &getTypeName() const;
		virtual void internalize(Reader &reader);
	};

	class TextPiece {
		public:
		const size_t d_len;
		TextPiece(size_t len);
		virtual void read(Reader &reader) = 0;
		virtual std::string toString() const = 0;
		virtual void accept(Visitor &visitor) const = 0;
	};

	class LongPiece : public TextPiece {
		private:
		CHAR *d_buf;
		public:
		LongPiece(size_t len);
		~LongPiece();
		virtual void read(Reader &reader);
		virtual std::string toString() const;
		virtual std::wstring getText() const;
		virtual void accept(Visitor &visitor) const;
	};

	class ShortPiece : public TextPiece {
		private:
		SHORTCHAR *d_buf;
		public:
		ShortPiece(size_t len);
		~ShortPiece();
		virtual void read(Reader &reader);
		virtual std::string toString() const;
		virtual std::string getText() const;
		virtual void accept(Visitor &visitor) const;
	};

	class ViewPiece : public TextPiece {
		Store *d_view;
		public:
		ViewPiece(Store *view);
		virtual void read(Reader &reader);
		virtual std::string toString() const;
		virtual void accept(Visitor &visitor) const;
	};

	class StdTextModel : public TextModel {
		private:
		static const std::string TYPENAME;
		static const TypeProxy<StdTextModel> PROXY;
		std::vector<TextPiece *> d_pieces;

		public:
		StdTextModel(INTEGER id);
		static const std::string &getType();
		static const std::string *getSuper();
		virtual const std::string &getTypeName() const;
		virtual void internalize(Reader &reader);

		virtual std::string toString();
		virtual void accept(Visitor &visitor) const;
	};

} // namespace odc

#endif // _TEXTMODEL_H_
