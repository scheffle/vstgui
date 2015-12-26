#include "../ivalue.h"
#include "../../lib/dispatchlist.h"

//------------------------------------------------------------------------
namespace VSTGUI {
namespace Standalone {
namespace /*Anonymous*/ {

//------------------------------------------------------------------------
class DefaultValueStringConverter : public IValueStringConverter
{
public:
	static IValueStringConverter& instance ()
	{
		static DefaultValueStringConverter gInstance;
		return gInstance;
	}

	UTF8String valueAsString (IValue::Type value) const override
	{
		UTF8String result;
		if (value < 0. || value > 1.)
			return result;

		result = std::to_string (value);
		return result;
	}
	
	IValue::Type stringAsValue (const UTF8String& string) const override
	{
		IValue::Type value;
		std::istringstream sstream (string.get ());
		sstream.imbue (std::locale::classic ());
		sstream.precision (40);
		sstream >> value;
		if (value < 0. || value > 1.)
			return IValue::InvalidValue;
		return value;
	}
};

//------------------------------------------------------------------------
class Value : public IValue
{
public:
	Value (const IdStringPtr id, Type initialValue, const IValueStringConverter& stringConverter);

	void beginEdit () override;
	bool performEdit (Type newValue) override;
	void endEdit () override;
	
	void setActive (bool state) override;
	bool isActive () const override;
	
	Type getValue () const override;
	bool isEditing () const override;
	
	const IdStringPtr getID () const override;

	const IValueStringConverter& getStringConverter () const override;
	
	void registerListener (IValueListener* listener) override;
	void unregisterListener (IValueListener* listener) override;
private:
	std::string idString;
	Type value;
	bool active {true};
	uint32_t editCount {0};
	const IValueStringConverter& stringConverter;
	DispatchList<IValueListener> listeners;
};

//------------------------------------------------------------------------
class StepValue : public Value, public IStepValue, public IValueStringConverter
{
public:
	StepValue (const IdStringPtr id, StepType initialSteps, Type initialValue, const IValueStringConverter* stringConverter);

	bool performEdit (Type newValue) override;

	StepType getSteps () const override;
	IValue::Type stepToValue (StepType step) const override;
	StepType valueToStep (IValue::Type) const override;

	UTF8String valueAsString (IValue::Type value) const override;
	IValue::Type stringAsValue (const UTF8String& string) const override;
private:
	StepType steps;
};

//------------------------------------------------------------------------
Value::Value (const IdStringPtr id, Type initialValue, const IValueStringConverter& stringConverter)
: idString (id)
, value (initialValue)
, stringConverter (stringConverter)
{
}

//------------------------------------------------------------------------
void Value::beginEdit ()
{
	++editCount;
	
	if (editCount == 1)
	{
		listeners.forEach ([this] (IValueListener* l) {
			l->onBeginEdit (*this);
		});
	}
}

//------------------------------------------------------------------------
bool Value::performEdit (Type newValue)
{
	if (newValue < 0. || newValue > 1.)
		return false;
	if (newValue == value)
		return true;
	value = newValue;
	
	listeners.forEach ([this] (IValueListener* l) {
		l->onPerformEdit (*this, value);
	});
	
	return true;
}

//------------------------------------------------------------------------
void Value::endEdit ()
{
	vstgui_assert (editCount > 0);
	--editCount;

	if (editCount == 0)
	{
		listeners.forEach ([this] (IValueListener* l) {
			l->onEndEdit (*this);
		});
	}
}

//------------------------------------------------------------------------
void Value::setActive (bool state)
{
	if (state == active)
		return;
	active = state;

	listeners.forEach ([this] (IValueListener* l) {
		l->onStateChange (*this);
	});
}

//------------------------------------------------------------------------
bool Value::isActive () const
{
	return active;
}

//------------------------------------------------------------------------
Value::Type Value::getValue () const
{
	return value;
}

//------------------------------------------------------------------------
bool Value::isEditing () const
{
	return editCount != 0;
}

//------------------------------------------------------------------------
const IdStringPtr Value::getID () const
{
	return idString.data ();
}

//------------------------------------------------------------------------
const IValueStringConverter& Value::getStringConverter () const
{
	return stringConverter;
}

//------------------------------------------------------------------------
void Value::registerListener (IValueListener* listener)
{
	listeners.add (listener);
}

//------------------------------------------------------------------------
void Value::unregisterListener (IValueListener* listener)
{
	listeners.remove (listener);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
StepValue::StepValue (const IdStringPtr id, StepType initialSteps, Type initialValue, const IValueStringConverter* stringConverter)
: Value (id, initialValue, stringConverter ? *stringConverter : *this)
, steps (initialSteps - 1)
{
}

//------------------------------------------------------------------------
bool StepValue::performEdit (Type newValue)
{
	return Value::performEdit (stepToValue (valueToStep (newValue)));
}

//------------------------------------------------------------------------
StepValue::StepType StepValue::getSteps () const
{
	return steps + 1;
}

//------------------------------------------------------------------------
IValue::Type StepValue::stepToValue (StepType step) const
{
	return static_cast<IValue::Type> (step) / static_cast<IValue::Type> (steps);
}

//------------------------------------------------------------------------
StepValue::StepType StepValue::valueToStep (IValue::Type value) const
{
	return std::min (steps, static_cast<StepType> (value * static_cast<IValue::Type> (steps + 1)));
}

//------------------------------------------------------------------------
UTF8String StepValue::valueAsString (IValue::Type value) const
{
	auto v = valueToStep (value);
	return {std::to_string (v)};
}

//------------------------------------------------------------------------
IValue::Type StepValue::stringAsValue (const UTF8String& string) const
{
	StepType v;
	std::istringstream sstream (string.get ());
	sstream.imbue (std::locale::classic ());
	sstream >> v;
	if (v > steps)
		return IValue::InvalidValue;
	return stepToValue (v);
}

} // Anonymous

//------------------------------------------------------------------------
ValuePtr IValue::make (const IdStringPtr id, Type initialValue, const IValueStringConverter* stringConverter)
{
	vstgui_assert (id != nullptr);
	return std::make_shared<Value>(id, initialValue, stringConverter ? *stringConverter : DefaultValueStringConverter::instance ());
}

//------------------------------------------------------------------------
ValuePtr IStepValue::make (const IdStringPtr id, StepType initialSteps, IValue::Type initialValue, const IValueStringConverter* stringConverter)
{
	vstgui_assert (id != nullptr);
	return std::make_shared<StepValue>(id, initialSteps, initialValue, stringConverter);
}

//------------------------------------------------------------------------
} // Standalone
} // VSTGUI