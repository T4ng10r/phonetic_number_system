#include <gui/search_phonetic_representation/substitute_search_result_combobox.h>
#include <QtGui/QPainter>
#include <QtWidgets/QStylePainter>
#include <loggers.h>
#include <QtWidgets/QApplication>
#include <data/matching_words_search_types.h>
#include <QtWidgets/QStyle>
Q_DECLARE_METATYPE(MatchingWord);
#include <gui/search_phonetic_representation/custom_combobox_tools.h>

void SubstituteSearchResultComboBox::paintEvent(QPaintEvent * event)
{
	int start_pos(0), prev_pos(0);
	QStylePainter painter(this);
	painter.setPen(palette().color(QPalette::Text));

	// draw the combobox frame, focusrect and selected etc.
	QStyleOptionComboBox opt;
	initStyleOption(&opt);
	painter.drawComplexControl(QStyle::CC_ComboBox, opt);

	QStyleOptionComboBox cmb;
	initStyleOption(&cmb);

	QString text_;
	QFontMetrics font_metric(font());

	QModelIndex index = model()->index(currentIndex(),0);
	QString text = model()->data(index).toString();
	MatchingWord success_word = model()->data(index,Qt::UserRole).value<MatchingWord>();

	QPen standard_pen = painter.pen();
	standard_pen.setColor(palette().color(QPalette::WindowText));
	QPen special_pen = painter.pen();
	special_pen.setColor(QColor(Qt::red));
	special_pen.setWidth(special_pen.width()+1);
	QRect rect_ = painter.style()->proxy()->subControlRect(QStyle::CC_ComboBox, &cmb, QStyle::SC_ComboBoxEditField, this);

	current_width = rect_.left();
	for(char letter : success_word.matchingLetters)
	{
		start_pos = text.indexOf(letter, prev_pos, Qt::CaseInsensitive);
		if (start_pos<0) break;

		text_ = text.mid(prev_pos, start_pos-prev_pos);
		drawItem(&painter, rect_ , text_, font_metric.width(text_), standard_pen, current_width);
		prev_pos = start_pos;

		text_ = text.mid(prev_pos, 1);
		drawItem(&painter, rect_ , text_, font_metric.width(text_), special_pen, current_width);
		prev_pos++;
	}
	text_ = text.mid(prev_pos);
	drawItem(&painter, rect_ , text_, font_metric.width(text_)+5, standard_pen, current_width);
}
	
