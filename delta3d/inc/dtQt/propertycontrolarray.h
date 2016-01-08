/* -*-c++-*-
 * testAPP - Using 'The MIT License'
 * Copyright (C) 2014, Caper Holdings LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef DELTA_PROPERTY_CONTROL_ARRAY_H
#define DELTA_PROPERTY_CONTROL_ARRAY_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDE DIRECTIVES
////////////////////////////////////////////////////////////////////////////////
#include <dtQt/export.h>
#include <dtQt/basepropertycontrol.h>
#include <dtCore/arrayactorpropertybase.h>



////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
////////////////////////////////////////////////////////////////////////////////
namespace Ui
{
   class PropertyControlArray;
}



namespace dtQt
{
   /////////////////////////////////////////////////////////////////////////////
   // CLASS CODE
   /////////////////////////////////////////////////////////////////////////////
   class DT_QT_EXPORT PropertyControlArray : public BasePropertyControl
   {
      Q_OBJECT
   public:
      typedef BasePropertyControl BaseClass;

      typedef dtCore::ArrayActorPropertyBase ArrayProperty;

      PropertyControlArray(QWidget* parent = NULL);

      virtual ~PropertyControlArray();

      /*virtual*/ QObject* GetControlUI();

      /*virtual*/ QLabel* GetLabel() const;

      /**
       * Update the UI values from the referenced data.
       */
      /*virtual*/ void UpdateUI(const PropertyType& prop);

      /**
       * Update the referenced data with values from the UI.
       */
      /*virtual*/ void UpdateData(PropertyType& propToUpdate);

      void UpdateButtons();

      void InsertItem(int index);
      void RemoveItem(int index);

      void SetCurrentIndex(int index);
      int GetCurrentIndex() const;

      int GetItemCount() const;

      ArrayProperty* GetArrayProperty() const;

   public slots:
      void OnAdd();
      void OnRemove();
      void OnMoveUp();
      void OnMoveDown();
      void OnArraySizeChanged(int newSize);
      void OnCurrentIndexChanged(int curRow, int curColumn, int prevRow, int prevColumn);

   signals:
      void SignalCurrentIndexChanged(int index);

   protected:
      /*virtual*/ void CreateConnections();

      Ui::PropertyControlArray* mUI;
   };
}

#endif