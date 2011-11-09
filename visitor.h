#ifndef _VISITOR_H_
#define _VISITOR_H_

namespace odc {

	class ShortPiece;
	class LongPiece;

	/**
	 * Visitor role of the Visitor design pattern.
	 * Can be used to transform the document tree.
	 */
	class Visitor {
		public:
		/**
		 * A "part" that can contain other elements starts here.
		 * This can be an StdTextModel or a ViewPiece or another container.
		 */
		virtual void partStart() = 0;
		/**
		 * A "part" ends here.
		 */
		virtual void partEnd() = 0;
		/**
		 * A left fold has been found.
		 * If the fold is collapsed, the first part that follows is the
		 * "hidden" part, otherwise the first part that follows is the
		 * "alternative" text. The rest is the vice versa.
		 * This is the main reason we even need a visitor.
		 * @param collapsed true if the fold is in collapsed form.
		 */
		virtual void foldLeft(bool collapsed) = 0;
		/**
		 * A right fold has been found.
		 */
		virtual void foldRight() = 0;
		/**
		 * A text piece has been found (8-bit characters).
		 */
		virtual void textShortPiece(const ShortPiece *piece) = 0;
		/**
		 * A text piece has been found (16-bit characters).
		 */
		virtual void textLongPiece(const LongPiece *piece) = 0;
	};
}

#endif // _VISITOR_H_
