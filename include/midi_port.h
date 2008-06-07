/*
 * midi_port.h - abstraction of MIDI-ports which are part of LMMS's MIDI-
 *               sequencing system
 *
 * Copyright (c) 2005-2008 Tobias Doerffel <tobydox/at/users.sourceforge.net>
 * 
 * This file is part of Linux MultiMedia Studio - http://lmms.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */


#ifndef _MIDI_PORT_H
#define _MIDI_PORT_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPair>

#include "midi.h"
#include "automatable_model.h"


class midiClient;
class midiEventProcessor;
class midiTime;


// class for abstraction of MIDI-port
class midiPort : public model, public serializingObject
{
	Q_OBJECT
	mapPropertyFromModel(int,inputChannel,setInputChannel,
							m_inputChannelModel);
	mapPropertyFromModel(int,outputChannel,setOutputChannel,
							m_outputChannelModel);
	mapPropertyFromModel(int,inputController,setInputController,
						m_inputControllerModel);
	mapPropertyFromModel(int,outputController,setOutputController,
						m_outputControllerModel);
	mapPropertyFromModel(bool,isReadable,setReadable,m_readableModel);
	mapPropertyFromModel(bool,isWritable,setWritable,m_writableModel);
public:
	typedef QMap<QString, bool> map;

	enum Modes
	{
		Disabled,	// don't route any MIDI-events (default)
		Input,		// from MIDI-client to MIDI-event-processor
		Output,		// from MIDI-event-processor to MIDI-client
		Duplex		// both directions
	} ;

	midiPort( const QString & _name,
			midiClient * _mc,
			midiEventProcessor * _mep,
			model * _parent = NULL,
			track * _track = NULL,
			Modes _mode = Disabled );
	virtual ~midiPort();

	inline const QString & name( void ) const
	{
		return( m_name );
	}

	void setName( const QString & _name );

	inline Modes mode( void ) const
	{
		return( m_mode );
	}

	void setMode( Modes _mode );

	inline void enableDefaultVelocityForInEvents( const bool _on )
	{
		m_defaultVelocityInEnabledModel.setValue( _on );
	}

	inline void enableDefaultVelocityForOutEvents( const bool _on )
	{
		m_defaultVelocityOutEnabledModel.setValue( _on );
	}


	void processInEvent( const midiEvent & _me, const midiTime & _time );
	void processOutEvent( const midiEvent & _me, const midiTime & _time );


	virtual void saveSettings( QDomDocument & _doc, QDomElement & _parent );
	virtual void loadSettings( const QDomElement & _this );

	virtual QString nodeName( void ) const
	{
		return( "midiport" );
	}

	void subscribeReadablePort( const QString & _port,
							bool _subscribe = TRUE );
	void subscribeWriteablePort( const QString & _port,
							bool _subscribe = TRUE );

	const map & readablePorts( void ) const
	{
		return( m_readablePorts );
	}

	const map & writablePorts( void ) const
	{
		return( m_writablePorts );
	}


signals:
	void readablePortsChanged( void );
	void writeablePortsChanged( void );
	void modeChanged( void );


public slots:
	void updateMidiPortMode( void );


private slots:
	void updateReadablePorts( void );
	void updateWriteablePorts( void );


private:
	midiClient * m_midiClient;
	midiEventProcessor * m_midiEventProcessor;

	QString m_name;	// TODO: replace with model-name-property!

	Modes m_mode;

	intModel m_inputChannelModel;
	intModel m_outputChannelModel;
	intModel m_inputControllerModel;
	intModel m_outputControllerModel;
	boolModel m_readableModel;
	boolModel m_writableModel;
	boolModel m_defaultVelocityInEnabledModel;
	boolModel m_defaultVelocityOutEnabledModel;

	map m_readablePorts;
	map m_writablePorts;


	friend class controllerConnectionDialog;
	friend class instrumentMidiIOView;

} ;



#endif
