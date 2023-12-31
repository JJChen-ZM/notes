/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_EditText.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_TextIterator.h"
#include "MyGUI_IRenderTarget.h"
#include "MyGUI_FontData.h"
#include "MyGUI_CommonStateInfo.h"

namespace MyGUI
{

	const size_t SIMPLETEXT_COUNT_VERTEX = 32 * VertexQuad::VertexCount;

	EditText::EditText() :
		mCountVertex(SIMPLETEXT_COUNT_VERTEX)
	{
		mVertexFormat = RenderManager::getInstance().getVertexFormat();
	}

	void EditText::setVisible(bool _visible)
	{
		if (mVisible == _visible)
			return;
		mVisible = _visible;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void EditText::_correctView()
	{
		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void EditText::_setAlign(const IntSize& _oldsize)
	{
		if (mWordWrap)
		{
			// передается старая координата всегда
			int width = mCroppedParent->getWidth();
			if (mOldWidth != width)
			{
				mOldWidth = width;
				mTextOutDate = true;
			}
		}

		// первоначальное выравнивание
		if (mAlign.isHStretch())
		{
			// растягиваем
			mCoord.width = mCoord.width + (mCroppedParent->getWidth() - _oldsize.width);
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isRight())
		{
			// двигаем по правому краю
			mCoord.left = mCoord.left + (mCroppedParent->getWidth() - _oldsize.width);
		}
		else if (mAlign.isHCenter())
		{
			// выравнивание по горизонтали без растяжения
			mCoord.left = (mCroppedParent->getWidth() - mCoord.width) / 2;
		}

		if (mAlign.isVStretch())
		{
			// растягиваем
			mCoord.height = mCoord.height + (mCroppedParent->getHeight() - _oldsize.height);
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isBottom())
		{
			// двигаем по нижнему краю
			mCoord.top = mCoord.top + (mCroppedParent->getHeight() - _oldsize.height);
		}
		else if (mAlign.isVCenter())
		{
			// выравнивание по вертикали без растяжения
			mCoord.top = (mCroppedParent->getHeight() - mCoord.height) / 2;
		}

		mCurrentCoord = mCoord;
		_updateView();
	}

	void EditText::_updateView()
	{
		bool margin = _checkMargin();

		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// вьюпорт стал битым
		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside())
			{
				// запоминаем текущее состояние
				mIsMargin = margin;

				// обновить перед выходом
				if (nullptr != mNode)
					mNode->outOfDate(mRenderItem);
				return;
			}
		}

		// мы обрезаны или были обрезаны
		if (mIsMargin || margin)
		{
			mCurrentCoord.width = _getViewWidth();
			mCurrentCoord.height = _getViewHeight();
		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void EditText::setCaption(const UString& _value)
	{
		mCaption = _value;
		mUtf32Caption = mCaption.asUTF32();
		mTextOutDate = true;

		checkVertexSize();

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void EditText::checkVertexSize()
	{
		// reallocate if we need more vertices (extra vertices for selection * 2 and cursor)
		size_t need = (mUtf32Caption.size() * (mShadow ? 3 : 2) + 2) * VertexQuad::VertexCount;
		if (mCountVertex < need)
		{
			mCountVertex = need + SIMPLETEXT_COUNT_VERTEX;
			if (nullptr != mRenderItem)
				mRenderItem->reallockDrawItem(this, mCountVertex);
		}
	}

	unsigned int EditText::getMixedNativeAlpha(float secondAlpha) const
	{
		return (uint8)(mAlpha * secondAlpha * 255) << 24;
	}

	const UString& EditText::getCaption() const
	{
		return mCaption;
	}

	void EditText::setTextColour(const Colour& _value)
	{
		mManualColour = true;
		_setTextColour(_value);
	}

	void EditText::_setTextColour(const Colour& _value)
	{
		if (mColour == _value)
			return;

		mColour = _value;
		mCurrentColourNative = texture_utility::toNativeColour(mColour, mVertexFormat);

		mCurrentColourNative = (mCurrentColourNative & 0x00FFFFFF) | getMixedNativeAlpha(mColour.alpha);
		mInverseColourNative = mCurrentColourNative ^ 0x00FFFFFF;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	const Colour& EditText::getTextColour() const
	{
		return mColour;
	}

	void EditText::setAlpha(float _value)
	{
		if (mAlpha == _value)
			return;
		mAlpha = _value;

		mCurrentColourNative = (mCurrentColourNative & 0x00FFFFFF) | getMixedNativeAlpha(mColour.alpha);
		mShadowColourNative = (mShadowColourNative & 0x00FFFFFF) | getMixedNativeAlpha(mShadowColour.alpha);
		mInverseColourNative = mCurrentColourNative ^ 0x00FFFFFF;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void EditText::setFontName(std::string_view _value)
	{
		mTexture = nullptr;
		mFont = FontManager::getInstance().getByName(_value);
		if (mFont != nullptr)
		{
			mTexture = mFont->getTextureFont();

			// set default font height
			if (mFont->getDefaultHeight() != 0 && !mCustomFontHeight)
			{
				mFontHeight = mFont->getDefaultHeight();
			}
		}

		mTextOutDate = true;

		// если мы были приаттаченны, то удаляем себя
		if (nullptr != mRenderItem)
		{
			mRenderItem->removeDrawItem(this);
			mRenderItem = nullptr;
		}

		// если есть текстура, то приаттачиваемся
		if (nullptr != mTexture && nullptr != mNode)
		{
			mRenderItem = mNode->addToRenderItem(mTexture, false, false);
			mRenderItem->addDrawItem(this, mCountVertex);
		}

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	std::string_view EditText::getFontName() const
	{
		return mFont->getResourceName();
	}

	void EditText::setFontHeight(int _value)
	{
		mCustomFontHeight = _value;
		if (!mCustomFontHeight && mFont)
			mFontHeight = mFont->getDefaultHeight();
		else
			mFontHeight = _value;
		mTextOutDate = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	int EditText::getFontHeight() const
	{
		return mFontHeight;
	}

	void EditText::createDrawItem(ITexture* _texture, ILayerNode* _node)
	{
		mNode = _node;
		// если уже есть текстура, то атачимся, актуально для смены леера
		if (nullptr != mTexture)
		{
			MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");

			mRenderItem = mNode->addToRenderItem(mTexture, false, false);
			mRenderItem->addDrawItem(this, mCountVertex);
		}
	}

	void EditText::destroyDrawItem()
	{
		if (nullptr != mRenderItem)
		{
			mRenderItem->removeDrawItem(this);
			mRenderItem = nullptr;
		}
		mNode = nullptr;
	}

	size_t EditText::getTextSelectionStart() const
	{
		return mStartSelect;
	}

	size_t EditText::getTextSelectionEnd() const
	{
		return mEndSelect;
	}

	void EditText::setTextSelection(size_t _start, size_t _end)
	{
		mStartSelect = _start;
		mEndSelect = _end;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	bool EditText::getSelectBackground() const
	{
		return mBackgroundNormal;
	}

	void EditText::setSelectBackground(bool _normal)
	{
		if (mBackgroundNormal == _normal)
			return;
		mBackgroundNormal = _normal;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	bool EditText::isVisibleCursor() const
	{
		return mVisibleCursor;
	}

	void EditText::setVisibleCursor(bool _value)
	{
		if (mVisibleCursor == _value)
			return;
		mVisibleCursor = _value;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	size_t EditText::getCursorPosition() const
	{
		return mCursorPosition;
	}

	void EditText::setCursorPosition(size_t _index)
	{
		if (mCursorPosition == _index)
			return;
		mCursorPosition = _index;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void EditText::setTextAlign(Align _value)
	{
		mTextAlign = _value;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	Align EditText::getTextAlign() const
	{
		return mTextAlign;
	}

	IntSize EditText::getTextSize() const
	{
		// если нуно обновить, или изменились пропорции экрана
		if (mTextOutDate)
			updateRawData();

		IntSize size = mTextView.getViewSize();
		// плюс размер курсора
		if (mIsAddCursorWidth)
			size.width += 2;

		if (mShadow)
		{
			if (!mIsAddCursorWidth)
				size.width++;
			size.height++;
		}

		return size;
	}

	const VectorLineInfo& EditText::getLineInfo() const
	{
		return mTextView.getData();
	}

	void EditText::setViewOffset(const IntPoint& _point)
	{
		mViewOffset = _point;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	IntPoint EditText::getViewOffset() const
	{
		return mViewOffset;
	}

	size_t EditText::getCursorPosition(const IntPoint& _point) const
	{
		if (nullptr == mFont)
			return 0;

		if (mTextOutDate)
			updateRawData();

		IntPoint point = _point;
		point -= mCroppedParent->getAbsolutePosition();
		point += mViewOffset;
		point -= mCoord.point();

		return mTextView.getCursorPosition(point);
	}

	IntCoord EditText::getCursorCoord(size_t _position) const
	{
		if (nullptr == mFont)
			return {};

		if (mTextOutDate)
			updateRawData();

		IntPoint point = mTextView.getCursorPoint(_position);
		point += mCroppedParent->getAbsolutePosition();
		point -= mViewOffset;
		point += mCoord.point();

		return {point.left, point.top, 2, mFontHeight};
	}

	void EditText::setShiftText(bool _value)
	{
		if (mShiftText == _value)
			return;
		mShiftText = _value;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void EditText::setWordWrap(bool _value)
	{
		mWordWrap = _value;
		mTextOutDate = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void EditText::updateRawData() const
	{
		if (nullptr == mFont)
			return;
		// сбрасывам флаги
		mTextOutDate = false;

		int width = -1;
		if (mWordWrap)
		{
			width = mCoord.width;
			// обрезать слова нужно по шарине, которую мы реально используем
			if (mIsAddCursorWidth)
				width -= 2;
		}

		mTextView.update(mUtf32Caption, mFont, mFontHeight, mTextAlign, mVertexFormat, width);
	}

	void EditText::setStateData(IStateInfo* _data)
	{
		EditTextStateInfo* data = _data->castType<EditTextStateInfo>();
		if (!mManualColour && data->getColour() != Colour::Zero)
			_setTextColour(data->getColour());
		setShiftText(data->getShift());
	}

	void EditText::doRender()
	{
		if (nullptr == mFont || !mVisible || mEmptyView)
			return;

		if (mRenderItem->getCurrentUpdate() || mTextOutDate)
			updateRawData();

		Vertex* vertex = mRenderItem->getCurrentVertexBuffer();

		const RenderTargetInfo& renderTargetInfo = mRenderItem->getRenderTarget()->getInfo();

		// колличество отрисованных вершин
		size_t vertexCount = 0;

		// текущие цвета
		uint32 colour = mCurrentColourNative;
		uint32 inverseColour = mInverseColourNative;
		uint32 selectedColour = mInvertSelect ? inverseColour : colour | 0x00FFFFFF;

		const VectorLineInfo& textViewData = mTextView.getData();

		float top = (float)(-mViewOffset.top + mCoord.top);

		FloatRect vertexRect;

		const FloatRect& selectedUVRect =
			mFont->getGlyphInfo(mBackgroundNormal ? FontCodeType::Selected : FontCodeType::SelectedBack)->uvRect;

		size_t index = 0;

		for (const auto& line : textViewData)
		{
			float left = (float)(line.offset - mViewOffset.left + mCoord.left);

			for (const auto& sym : line.symbols)
			{
				if (sym.isColour())
				{
					colour = sym.getColour() | (colour & 0xFF000000);
					inverseColour = colour ^ 0x00FFFFFF;
					selectedColour = mInvertSelect ? inverseColour : colour | 0x00FFFFFF;
					continue;
				}

				// смещение текстуры для фона
				bool select = index >= mStartSelect && index < mEndSelect;

				float fullAdvance = sym.getBearingX() + sym.getAdvance();

				// Render the selection, if any, first.
				if (select)
				{
					vertexRect.set(left, top, left + fullAdvance, top + (float)mFontHeight);

					drawGlyph(renderTargetInfo, vertex, vertexCount, vertexRect, selectedUVRect, selectedColour);
				}

				// Render the glyph shadow, if any.
				if (mShadow)
				{
					vertexRect.left = left + sym.getBearingX() + 1.0f;
					vertexRect.top = top + sym.getBearingY() + 1.0f;
					vertexRect.right = vertexRect.left + sym.getWidth();
					vertexRect.bottom = vertexRect.top + sym.getHeight();

					drawGlyph(renderTargetInfo, vertex, vertexCount, vertexRect, sym.getUVRect(), mShadowColourNative);
				}

				// Render the glyph itself.
				vertexRect.left = left + sym.getBearingX();
				vertexRect.top = top + sym.getBearingY();
				vertexRect.right = vertexRect.left + sym.getWidth();
				vertexRect.bottom = vertexRect.top + sym.getHeight();

				drawGlyph(
					renderTargetInfo,
					vertex,
					vertexCount,
					vertexRect,
					sym.getUVRect(),
					(!select || !mInvertSelect) ? colour : inverseColour);

				left += fullAdvance;
				++index;
			}

			top += mFontHeight;
			++index;
		}

		// Render the cursor, if any, last.
		if (mVisibleCursor)
		{
			IntPoint point = mTextView.getCursorPoint(mCursorPosition) - mViewOffset + mCoord.point();
			const GlyphInfo* cursorGlyph = mFont->getGlyphInfo(static_cast<Char>(FontCodeType::Cursor));
			vertexRect.set(
				(float)point.left,
				(float)point.top,
				(float)point.left + cursorGlyph->width,
				(float)(point.top + mFontHeight));

			drawGlyph(
				renderTargetInfo,
				vertex,
				vertexCount,
				vertexRect,
				cursorGlyph->uvRect,
				mCurrentColourNative | 0x00FFFFFF);
		}

		// колличество реально отрисованных вершин
		mRenderItem->setLastVertexCount(vertexCount);
	}

	void EditText::setInvertSelected(bool _value)
	{
		if (mInvertSelect == _value)
			return;
		mInvertSelect = _value;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	bool EditText::getInvertSelected() const
	{
		return mInvertSelect;
	}

	bool EditText::getShadow() const
	{
		return mShadow;
	}

	void EditText::setShadow(bool _value)
	{
		mShadow = _value;
		mTextOutDate = true;

		checkVertexSize();

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void EditText::setShadowColour(const Colour& _value)
	{
		mShadowColour = _value;
		mShadowColourNative = texture_utility::toNativeColour(mShadowColour, mVertexFormat);

		mShadowColourNative = (mShadowColourNative & 0x00FFFFFF) | getMixedNativeAlpha(mShadowColour.alpha);

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	const Colour& EditText::getShadowColour() const
	{
		return mShadowColour;
	}

	void EditText::drawQuad(
		Vertex*& _vertex,
		size_t& _vertexCount,
		const FloatRect& _vertexRect,
		float _vertexZ,
		const FloatRect& _textureRect,
		uint32 _colour) const
	{
		_vertex[0].x = _vertexRect.left;
		_vertex[0].y = _vertexRect.top;
		_vertex[0].z = _vertexZ;
		_vertex[0].colour = _colour;
		_vertex[0].u = _textureRect.left;
		_vertex[0].v = _textureRect.top;

		_vertex[2].x = _vertexRect.left;
		_vertex[2].y = _vertexRect.bottom;
		_vertex[2].z = _vertexZ;
		_vertex[2].colour = _colour;
		_vertex[2].u = _textureRect.left;
		_vertex[2].v = _textureRect.bottom;

		_vertex[1].x = _vertexRect.right;
		_vertex[1].y = _vertexRect.top;
		_vertex[1].z = _vertexZ;
		_vertex[1].colour = _colour;
		_vertex[1].u = _textureRect.right;
		_vertex[1].v = _textureRect.top;

		_vertex[3].x = _vertexRect.right;
		_vertex[3].y = _vertexRect.top;
		_vertex[3].z = _vertexZ;
		_vertex[3].colour = _colour;
		_vertex[3].u = _textureRect.right;
		_vertex[3].v = _textureRect.top;

		_vertex[5].x = _vertexRect.left;
		_vertex[5].y = _vertexRect.bottom;
		_vertex[5].z = _vertexZ;
		_vertex[5].colour = _colour;
		_vertex[5].u = _textureRect.left;
		_vertex[5].v = _textureRect.bottom;

		_vertex[4].x = _vertexRect.right;
		_vertex[4].y = _vertexRect.bottom;
		_vertex[4].z = _vertexZ;
		_vertex[4].colour = _colour;
		_vertex[4].u = _textureRect.right;
		_vertex[4].v = _textureRect.bottom;

		_vertex += VertexQuad::VertexCount;
		_vertexCount += VertexQuad::VertexCount;
	}

	void EditText::drawGlyph(
		const RenderTargetInfo& _renderTargetInfo,
		Vertex*& _vertex,
		size_t& _vertexCount,
		FloatRect _vertexRect,
		FloatRect _textureRect,
		uint32 _colour) const
	{
		// символ залазиет влево
		float leftClip = (float)mCurrentCoord.left - _vertexRect.left;
		if (leftClip > 0.0f)
		{
			if ((float)mCurrentCoord.left < _vertexRect.right)
			{
				_textureRect.left += _textureRect.width() * leftClip / _vertexRect.width();
				_vertexRect.left += leftClip;
			}
			else
			{
				return;
			}
		}

		// символ залазиет вправо
		float rightClip = _vertexRect.right - (float)mCurrentCoord.right();
		if (rightClip > 0.0f)
		{
			if (_vertexRect.left < (float)mCurrentCoord.right())
			{
				_textureRect.right -= _textureRect.width() * rightClip / _vertexRect.width();
				_vertexRect.right -= rightClip;
			}
			else
			{
				return;
			}
		}

		// символ залазиет вверх
		float topClip = (float)mCurrentCoord.top - _vertexRect.top;
		if (topClip > 0.0f)
		{
			if ((float)mCurrentCoord.top < _vertexRect.bottom)
			{
				_textureRect.top += _textureRect.height() * topClip / _vertexRect.height();
				_vertexRect.top += topClip;
			}
			else
			{
				return;
			}
		}

		// символ залазиет вниз
		float bottomClip = _vertexRect.bottom - (float)mCurrentCoord.bottom();
		if (bottomClip > 0.0f)
		{
			if (_vertexRect.top < (float)mCurrentCoord.bottom())
			{
				_textureRect.bottom -= _textureRect.height() * bottomClip / _vertexRect.height();
				_vertexRect.bottom -= bottomClip;
			}
			else
			{
				return;
			}
		}

		float pix_left = mCroppedParent->getAbsoluteLeft() - _renderTargetInfo.leftOffset + _vertexRect.left;
		float pix_top = mCroppedParent->getAbsoluteTop() - _renderTargetInfo.topOffset + (mShiftText ? 1.0f : 0.0f) +
			_vertexRect.top;

		FloatRect vertexRect(
			((_renderTargetInfo.pixScaleX * pix_left + _renderTargetInfo.hOffset) * 2.0f) - 1.0f,
			-(((_renderTargetInfo.pixScaleY * pix_top + _renderTargetInfo.vOffset) * 2.0f) - 1.0f),
			((_renderTargetInfo.pixScaleX * (pix_left + _vertexRect.width()) + _renderTargetInfo.hOffset) * 2.0f) -
				1.0f,
			-(((_renderTargetInfo.pixScaleY * (pix_top + _vertexRect.height()) + _renderTargetInfo.vOffset) * 2.0f) -
			  1.0f));

		drawQuad(_vertex, _vertexCount, vertexRect, mNode->getNodeDepth(), _textureRect, _colour);
	}

} // namespace MyGUI
